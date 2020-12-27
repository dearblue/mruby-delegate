#ruby

require "yaml"

config = YAML.load <<'YAML'
  builds:
    host:
      defines: [MRB_NO_BOXING, MRB_INT64]
    host-int32:
      defines: [MRB_NO_BOXING, MRB_INT32]
    host-nan:
      defines: MRB_NAN_BOXING
    host++-word:
      defines: MRB_WORD_BOXING
      c++abi: true
YAML

begin
  require "mruby/source"
rescue LoadError
  $: << File.join(MRUBY_ROOT, "lib")
  require "mruby/source"
end

config["builds"].each_pair do |n, c|
  next if MRuby::Source::MRUBY_RELEASE_NO == 20001 && Array(c.dig("defines")).include?("MRB_NAN_BOXING")
  next if MRuby::Source::MRUBY_RELEASE_NO == 10400 && Array(c.dig("defines")).include?("MRB_INT32")
  next if MRuby::Source::MRUBY_RELEASE_NO == 10400 && Array(c.dig("defines")).include?("MRB_NAN_BOXING")
  next if MRuby::Source::MRUBY_RELEASE_NO == 10300 && Array(c.dig("defines")).include?("MRB_INT32")
  next if MRuby::Source::MRUBY_RELEASE_NO == 10300 && Array(c.dig("defines")).include?("MRB_NAN_BOXING")
  next if MRuby::Source::MRUBY_RELEASE_NO == 10300 && Array(c.dig("defines")).include?("MRB_WORD_BOXING")

  MRuby::Build.new(n) do |conf|
    toolchain :gcc

    conf.build_dir = File.join("build", c["build_dir"] || name)

    enable_debug
    enable_test
    enable_cxx_abi if c["c++abi"]

    cc.defines << [*c["defines"]]
    cc.flags << [*c["cflags"]]

    Array(config.dig("common", "gems")).each { |*g| gem *g }
    Array(c["gems"]).each { |*g| gem *g }

    gem __dir__ do |g|
      if g.cc.command =~ /\b(?:g?cc|clang)\d*\b/
        g.cxx.flags << "-std=c++11"
        g.cxx.flags << %w(-pedantic -Wall -Wextra -Wno-unused-parameter)
        g.cc.flags << (c["c++abi"] ? "-std=c++11" : "-std=c11")
        g.cc.flags << %w(-Wpedantic -Wall -Wextra -Wno-unused-parameter)
        g.cc.flags << %w(-Wno-declaration-after-statement) unless c["c++abi"]
      end
    end
  end
end
