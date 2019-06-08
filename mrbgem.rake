#!ruby

MRuby::Gem::Specification.new("mruby-delegate") do |s|
  s.summary = "Delegator implementation for mruby"
  version = File.read(File.join(__dir__, "README.md")).scan(/^\s*[\-\*] version:\s*(\d+(?:\.\w+)+)/i).flatten[-1]
  s.version = version if version
  s.license = "CC0"
  s.author  = "dearblue"
  s.homepage = "https://github.com/dearblue/mruby-delegate"

  if cc.command =~ /\b(?:g?cc|clang)\d*\b/
    cc.flags << %w(-Wno-declaration-after-statement)

    if false
      cc.flags << %w(-Weverything)
      cc.flags << %w(
        -Wpadded -Wc11-extensions -Wc++11-extensions -Wc99-extensions
        -Wsign-conversion -Wdocumentation -Wdocumentation-unknown-command
        -Wswitch-enum -Wunused-parameter -Wunreachable-code-return
        -Wmissing-prototypes -Wunused-macros -Wgnu-anonymous-struct
        -Wold-style-cast -Wmissing-field-initializers
      ).map { |w| w.insert(2, "no-") }
    end
  end
end
