#!ruby

class Delegator
  def method_missing(*args, &block)
    __getobj__.__send__(*args, &block)
  end
end
