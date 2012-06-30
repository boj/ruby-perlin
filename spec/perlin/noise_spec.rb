
require "./../helper.rb"

describe Perlin::Noise do
  before :each do
    @noise = Perlin::Noise.new 123, 1.5, 2
  end

  it "should have persistence set correctly" do
    @noise.persistence.should eq 1.5
  end

  it "should have octave set correctly" do
    @noise.octave.should eq 2
    @noise.octave.should be_kind_of Integer
  end

  describe "[]" do
    it "should fail if given too few arguments" do
      ->{ @noise[0] }.should raise_error ArgumentError
    end

    it "should fail if given too many arguments" do
      ->{ @noise[0, 0, 0, 0] }.should raise_error ArgumentError
    end

    describe "[](x, y)" do
      it "should return the appropriate value" do
        @noise[0, 0].should eq 0.5729669332504272
      end
    end

    describe "[](x, y, z)" do
      it "should return the appropriate value" do
        @noise[0, 0, 0].should eq 0.4035601392388344
      end
    end
  end

  describe "chunk" do
    describe "chunk(x, y, size_x, size_y)" do
      it "should return the appropriate values" do
        chunk = @noise.chunk 1, 2, 3, 4
        chunk.should eq [
                            [0.23042351007461548, 1.1444950252771378, -0.18201526999473572, 0.10285091400146484],
                            [-0.7967360764741898, -2.9295682907104492e-05, 0.009638801217079163, 1.5842265486717224],
                            [-0.8445602059364319, -0.08269979804754257, -0.15433502569794655, 0.7924925200641155]
                        ]

      end

      it "should fail if given negative size_x" do
        ->{ @noise.chunk 0, 0, -1, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_y" do
        ->{ @noise.chunk 0, 0, 0, -1 }.should raise_error ArgumentError
      end
    end

    describe "chunk(x, y, z, size_x, size_y, size_z)" do
      it "should return the appropriate values" do
        chunk = @noise.chunk 6, 5, 4, 3, 2, 1
        chunk.should eq [
                            [
                                [-0.32017043232917786], [-0.5810140073299408]
                            ],
                            [
                                [-0.4722275957465172], [1.1914344131946564]
                            ],
                            [
                                [1.1328794583678246], [1.338328868150711]
                            ]
                        ]

      end

      it "should fail if given negative size_x" do
        ->{ @noise.chunk 0, 0, 0, -1, 0, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_y" do
        ->{ @noise.chunk 0, 0, 0, 0, -1, 0 }.should raise_error ArgumentError
      end

      it "should fail if given negative size_z" do
        ->{ @noise.chunk 0, 0, 0, 0, 0, -1 }.should raise_error ArgumentError
      end
    end
  end
end