
require "./../helper.rb"

describe Perlin::Generator do
  before :each do
    @perlin = Perlin::Generator.new 123, 1.5, 2
  end

  it "should have seed set correctly" do
    @perlin.seed.should eq 123
    @perlin.seed.should be_kind_of Integer
  end

  it "should have seed set uniquely on all generators" do
    # This was a bug in the original code!
    g = Perlin::Generator.new 99, 1, 1
    g.seed.should eq 99
    g.seed.should be_kind_of Integer

    @perlin.seed.should eq 123
    g[0, 0].should_not eq @perlin[0, 0]
  end

  it "should have persistence set correctly" do
    @perlin.persistence.should eq 1.5
  end

  it "should have octave set correctly" do
    @perlin.octave.should eq 2
    @perlin.octave.should be_kind_of Integer
  end

  describe "seed=" do
    it "should set seed correctly" do
      @perlin.seed = 12
      @perlin.seed.should eq 12
      @perlin.seed.should be_kind_of Integer
    end
  end

  describe "persistence=" do
    it "should set persistence correctly" do
      @perlin.persistence = 12
      @perlin.persistence.should eq 12
      @perlin.persistence.should be_kind_of Float
    end
  end

  describe "octave=" do
    it "should set octave correctly" do
      @perlin.octave = 12
      @perlin.octave.should eq 12
      @perlin.octave.should be_kind_of Integer
    end
  end

  describe "[]" do
    it "should fail if given too few arguments" do
      ->{ @perlin[0] }.should raise_error ArgumentError
    end

    it "should fail if given too many arguments" do
      ->{ @perlin[0, 0, 0, 0] }.should raise_error ArgumentError
    end

    describe "[](x, y)" do
      it "should return the appropriate value" do
        @perlin[0, 0].should eq -1.0405873507261276
      end
    end

    describe "[](x, y, z)" do
      it "should return the appropriate value" do
        @perlin[0, 0, 0].should eq -1.5681833028793335
      end
    end
  end

  describe "chunk" do
    describe "chunk(x, y, size_x, size_y)" do
      it "should return the appropriate values" do
        chunk = @perlin.chunk 1, 2, 3, 4
        chunk.should eq [[-2.014809340238571, -0.7094215080142021, -0.5946878045797348, 0.4915006756782532], [-1.4068767204880714, -0.732808068394661, 0.07362580299377441, -0.325466126203537], [-0.857817449606955, -1.940980076789856, -0.5687579363584518, 1.4209578335285187]]
      end

      it "should fail if given negative size_x" do
        ->{ @perlin.chunk 0, 0, -1, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_y" do
        ->{ @perlin.chunk 0, 0, 0, -1 }.should raise_error ArgumentError
      end
    end

    describe "chunk(x, y, z, size_x, size_y, size_z)" do
      it "should return the appropriate values" do
        chunk = @perlin.chunk 6, 5, 4, 3, 2, 1
        chunk.should eq [[[0.7522532045841217], [0.3314518630504608]], [[0.3198353797197342], [0.967293307185173]], [[1.1024393141269684], [0.5659154206514359]]]
      end

      it "should fail if given negative size_x" do
        ->{ @perlin.chunk 0, 0, 0, -1, 0, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_y" do
        ->{ @perlin.chunk 0, 0, 0, 0, -1, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_z" do
        ->{ @perlin.chunk 0, 0, 0, 0, 0, -1 }.should raise_error ArgumentError
      end
    end
  end
end