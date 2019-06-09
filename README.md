# mruby-delegate

mruby 上で利用可能な、移譲オブジェクトです。

CRuby の標準添付ライブラリである `delegate` を模倣しているつもりです。


## 注意と制限

CRuby と同じような実装にはなっていません。

  - `Delegator` クラスの直接の親は `BasicObject` で、カスタマイズされた `Kernel` オブジェクトは `include` されません。
  - `Delegator` インスタンスに移譲先オブジェクトのメソッドが定義されるのではなく、`method_missing` メソッド経由での呼び出しとなります。  
    これはヒープメモリの節約のためです。
  - CRuby で定義される `DelegatorClass` メソッドは未定義です。
  - CRuby で定義されるいくつかの `Delegator` インスタンスメソッドは未定義です。


## くみこみかた

`build_config.rb` に gem として追加して、mruby をビルドして下さい。

```ruby
MRuby::Build.new do |conf|
  conf.gem "mruby-delegate", github: "dearblue/mruby-delegate"
end
```

- - - -

mruby gem パッケージとして依存したい場合、`mrbgem.rake` に記述して下さい。

```ruby
# mrbgem.rake
MRuby::Gem::Specification.new("mruby-XXX") do |spec|
  ...
  spec.add_dependency "mruby-delegate", github: "dearblue/mruby-delegate"
end
```


## つかいかた

たぶん <https://docs.ruby-lang.org/ja/latest/library/delegate.html> の通りに動作します……するといいな。


## Specification

  - Package name: mruby-delegate
  - Version: 0.1
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
