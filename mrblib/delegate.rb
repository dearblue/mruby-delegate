#!ruby

class Delegator
  #def initialize(obj)
  #  __setobj__ obj

  #  methods = obj.methods
  #  specials = %i(__setobj__ __getobj__ method_missing)
  #  methods -= specials

  #  obj = nil # このスコープにおける元のオブジェクトへの参照を無効にする

  #  singleton_class.instance_eval do
  #    methods.each do |mid|
  #      define_method(mid) do |*args, &block|
  #        __getobj__.__send__(mid, *args, &block)
  #      end
  #    end
  #  end
  #end

  def method_missing(*args, &block)
    __getobj__.__send__(*args, &block)
  end

  #def __setobj__(obj)
  #  raise NotImplementedError, "need to define `__setobj__' method"
  #end

  #def __getobj__
  #  raise NotImplementedError, "need to define `__getobj__' method"
  #end
end
