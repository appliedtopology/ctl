require "formula"

class Ctlib < Formula
  url "http://ctl.appliedtopology.org/r/v0.1.tgz"
  homepage ""
  #sha1 ""

  depends_on "cmake" => :build
  depends_on "tbb"
  depends_on "boost"
  depends_on "metis"

  def install
    system "cmake . #{std_cmake_parameters}" 
    system "make"
    system "make install"
  end
end
