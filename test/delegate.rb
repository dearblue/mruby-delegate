#!ruby

assert "Delegator" do
  assert_raise(NotImplementedError) { Delegator.new(nil) }
end

assert "SimpleDelegator" do
  target = [*1..20]
  delegator = SimpleDelegator.new(target)

  assert_equal target.inspect, delegator.inspect

  # more test!
end
