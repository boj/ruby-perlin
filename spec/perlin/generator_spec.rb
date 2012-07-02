
require File.expand_path("../../helper.rb", __FILE__)

describe Perlin::Generator do
  before :each do
    @classic = Perlin::Generator.new 123, 1.5, 2, :classic => true
    @simplex = Perlin::Generator.new 123, 1.5, 2
    @accuracy = 0.00001
  end

  it "should have seed set correctly" do
    @classic.seed.should eq 123
    @classic.seed.should be_kind_of Integer
  end

  it "should have seed set uniquely on all generators" do
    # This was a bug in the original code!
    g = Perlin::Generator.new 99, 1, 1
    g.seed.should eq 99
    g.seed.should be_kind_of Integer

    @classic.seed.should eq 123
    g[0, 0].should_not eq @classic[0, 0]
  end

  it "should have persistence set correctly" do
    @classic.persistence.should be_within(@accuracy).of 1.5
  end

  it "should have octave set correctly" do
    @classic.octave.should eq 2
    @classic.octave.should be_kind_of Integer
  end

  describe "SIMPLEX" do
    it "should have classic? set correctly" do
      @simplex.classic?.should be_false
    end
  end

  describe "CLASSIC" do
    it "should have classic? set correctly" do
      @classic.classic?.should be_true
    end
  end

  describe "seed=" do
    it "should set seed correctly" do
      @classic.seed = 12
      @classic.seed.should eq 12
      @classic.seed.should be_kind_of Integer
    end
  end

  describe "persistence=" do
    it "should set persistence correctly" do
      @classic.persistence = 12
      @classic.persistence.should eq 12
      @classic.persistence.should be_kind_of Float
    end
  end

  describe "octave=" do
    it "should set octave correctly" do
      @classic.octave = 12
      @classic.octave.should eq 12
      @classic.octave.should be_kind_of Integer
    end
  end

  describe "classic=" do
    it "should set classic? correctly" do
      @simplex.classic = true
      @simplex.classic?.should be_true
    end
  end

  describe "[]" do
    it "[x, y] should support float values" do
      @simplex[0, 0].should_not be_within(@accuracy).of @simplex[0.2, 0.2]
    end

    it "[x, y, z] should support float values" do
      @simplex[0, 0, 0].should_not be_within(@accuracy).of @simplex[0.2, 0.2, 0.2]
    end

    it "should fail if given too few arguments" do
      lambda { @classic[0] }.should raise_error ArgumentError
    end

    it "should fail if given too many arguments" do
      lambda { @classic[0, 0, 0, 0] }.should raise_error ArgumentError
    end

    describe "SIMPLEX" do
      describe "[](x, y)" do
        it "should return the appropriate value" do
          @simplex[0, 1].should be_within(@accuracy).of -0.48430206986846436
        end
      end

      describe "[](x, y, z)" do
        it "should return the appropriate value" do
          @simplex[0, 1, 2].should be_within(@accuracy).of -2.9663090652320816e-07
        end
      end
    end

    describe "CLASSIC" do
      describe "[](x, y)" do
        it "should return the appropriate value" do
          @classic[0, 0].should be_within(@accuracy).of -1.0405873507261276
        end
      end

      describe "[](x, y, z)" do
        it "should return the appropriate value" do
          @classic[0, 0, 0].should be_within(@accuracy).of -1.5681833028793335
        end
      end
    end
  end

  describe "chunk" do
    describe "chunk(x, y, size_x, size_y)" do
      describe "SIMPLEX" do
        it "should return the appropriate values" do
          chunk = @simplex.chunk 1, 2, 3, 4
          chunk.should eq [[0.3353644531228476, 0.19956992864608764, -0.14786987723329942, -0.015269053664457142], [0.06330469303329524, 0.14797919215181748, 0.6964946275780389, 0.16488039129141768], [0.14797919215181748, 0.3145651348893048, 0.21095079795844954, 3.090576098737001e-07]]
        end

        it "should work with a block" do
          arr = []
          @simplex.chunk(1, 2, 3, 4) do |h, x, y|
            arr << [h, x, y]
          end
          arr.should eq [[0.3353644531228476, 1, 2], [0.06330469303329524, 2, 2], [0.14797919215181748, 3, 2], [0.19956992864608764, 1, 3], [0.14797919215181748, 2, 3], [0.3145651348893048, 3, 3], [-0.14786987723329942, 1, 4], [0.6964946275780389, 2, 4], [0.21095079795844954, 3, 4], [-0.015269053664457142, 1, 5], [0.16488039129141768, 2, 5], [3.090576098737001e-07, 3, 5]]
        end
      end

      describe "CLASSIC" do
        it "should return the appropriate values" do
          chunk = @classic.chunk 1, 2, 3, 4
          chunk.should eq [[-2.014809340238571, -0.7094215080142021, -0.5946878045797348, 0.4915006756782532], [-1.4068767204880714, -0.732808068394661, 0.07362580299377441, -0.325466126203537], [-0.857817449606955, -1.940980076789856, -0.5687579363584518, 1.4209578335285187]]
        end

        it "should work with a block" do
          arr = []
          @classic.chunk(1, 2, 3, 4) do |h, x, y|
            arr << [h, x, y]
          end
          arr.should eq [[-2.014809340238571, 1, 2], [-1.4068767204880714, 2, 2], [-0.857817449606955, 3, 2], [-0.7094215080142021, 1, 3], [-0.732808068394661, 2, 3], [-1.940980076789856, 3, 3], [-0.5946878045797348, 1, 4], [0.07362580299377441, 2, 4], [-0.5687579363584518, 3, 4], [0.4915006756782532, 1, 5], [-0.325466126203537, 2, 5], [1.4209578335285187, 3, 5]]
        end
      end

      it "should fail if given negative size_x" do
        lambda { @classic.chunk 0, 0, -1, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_y" do
        lambda { @classic.chunk 0, 0, 0, -1 }.should raise_error ArgumentError
      end
    end

    describe "chunk(x, y, z, size_x, size_y, size_z)" do
      describe "SIMPLEX" do
        it "should return the appropriate values" do
          chunk = @simplex.chunk 6, 5, 4, 3, 2, 1
          chunk.should eq [[[0.0], [0.7600998644617093]], [[-0.7600998644617093], [0.6522208451958038]], [[0.0647275250791234], [0.0]]]
        end

        it "should work with a block" do
          arr = []
          @simplex.chunk(6, 5, 4, 3, 2, 1) do |h, x, y, z|
            arr << [h, x, y, z]
          end
          arr.should eq [[0.0, 6, 5, 4], [-0.7600998644617093, 7, 5, 4], [0.0647275250791234, 8, 5, 4], [0.7600998644617093, 6, 6, 4], [0.6522208451958038, 7, 6, 4], [0.0, 8, 6, 4]]
        end
      end

      describe "CLASSIC" do
        it "should return the appropriate values" do
          chunk = @classic.chunk 6, 5, 4, 3, 2, 1
          chunk.should eq [[[0.7522532045841217], [0.3314518630504608]], [[0.3198353797197342], [0.967293307185173]], [[1.1024393141269684], [0.5659154206514359]]]
        end

        it "should work with a block" do
          arr = []
          @classic.chunk(6, 5, 4, 3, 2, 1) do |h, x, y, z|
            arr << [h, x, y, z]
          end
          arr.should eq [[0.7522532045841217, 6, 5, 4], [0.3198353797197342, 7, 5, 4], [1.1024393141269684, 8, 5, 4], [0.3314518630504608, 6, 6, 4], [0.967293307185173, 7, 6, 4], [0.5659154206514359, 8, 6, 4]]
        end
      end

      it "should fail if given negative size_x" do
        lambda { @classic.chunk 0, 0, 0, -1, 0, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_y" do
        lambda { @classic.chunk 0, 0, 0, 0, -1, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_z" do
        lambda { @classic.chunk 0, 0, 0, 0, 0, -1 }.should raise_error ArgumentError
      end
    end
  end
end