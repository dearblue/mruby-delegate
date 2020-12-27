#!ruby

class Delegator
  def method_missing(...)
    __getobj__.__send__(...)
  end
end

