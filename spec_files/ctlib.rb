require "formula"

class Ctlib < Formula
  url "http://ctl.appliedtopology.org/r/v0.1.tgz"
  homepage "http://ctl.appliedtopology.org/r/v0.1.tgz"
  #sha1 ""

  depends_on "cmake" => :build
  depends_on "tbb"
  depends_on "boost"
  depends_on "metis"

  def install
    system "cmake . " 
    system "make"
    system "make install"
    bin.install Dir["bin/*"]
    man1.install Dir["man/*"]
    include.install Dir["include/*"]
    doc.install Dir["doc/*"]
    share.install Dir["examples/*]
  end
end
