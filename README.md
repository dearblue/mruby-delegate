# mruby-delegate

([日本語で書かれた原文 / Written original README in Japanese](README.ja.md))

Delegated object available on mruby.

I'm trying to mimic `Delegate`, which is a standard attachment library of CRuby.


## Cautions and limitations

It is not an implementation similar to CRuby.

  - The immediate parent of the `Delegator` class is `BasicObject`, and customized `Kernel` objects do not `include`.
  - The `Delegator` instance does not define the method of the delegate object, but it will be called via the `method_missing` method.
    This is to save heap memory.
  - The `DelegatorClass` method defined in CRuby is undefined.
  - Some `Delegator` instance methods defined in CRuby are undefined.


## How to install

Build mruby by adding it as a gem to `build_config.rb`.

```ruby
MRuby::Build.new do |conf|
  conf.gem "mruby-delegate", github: "dearblue/mruby-delegate"
end
```

- - - -

If you want to depend on the mruby gem package, write it in `mrbgem.rake`.

```ruby
# mrbgem.rake
MRuby::Gem::Specification.new("mruby-XXX") do |spec|
  ...
  spec.add_dependency "mruby-delegate", github: "dearblue/mruby-delegate"
end
```


## How to use

Maybe it works as <https://docs.ruby-lang.org/ja/latest/library/delegate.html> ... hopefully.


## Specification

  - Package name: mruby-delegate
  - Version: 0.2
  - Product quality: CONCEPT
  - Author: [dearblue](https://github.com/dearblue)
  - Project page: <https://github.com/dearblue/mruby-delegate>
  - Licensing: [Creative Commons Zero License (CC0; likely Public Domain)](LICENSE)
  - Required mruby version: 1.4.0 or later
  - Object code size: 2 Ki bytes on FreeBSD 12.0 AMD64 with clang
  - Heap usages per one delegate instance (on FreeBSD 12.0 AMD64):
      - 48 bytes: delegator object
      - 20×N bytes: delegated method table (N is count of string methods (probably more than 160))
      - about 3500 bytes: total
  - Dependency external mrbgems: (NONE)
  - Bundled C libraries (git-submodules): (NONE)
