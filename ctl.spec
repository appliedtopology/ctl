Name:       ctl
Version:    0.0.24
Release:    noop
Summary:    A computational topology library
License:    BSD-3
BuildArch:  x86_64

BuildRequires: boost-devel
BuildRequires: ann-devel
BuildRequires: tbb-devel
BuildRequires: metis-devel
BuildRequires: doxygen 

BuildRequires: boost
BuildRequires: ann
BuildRequires: tbb
BuildRequires: metis
BuildRequires: doxygen 

%description
A computational topology library
%prep
%setup -c -q -T -D -a 0

%build
mkdir build
cd build
cmake ..
make %{?_smp_flags}
 
%install
make install

%files

%changelog
