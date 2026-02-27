{pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation rec {
  pname = "Solitair";
  version = "3.2.3";

  src = pkgs.fetchFromGitHub {
    owner = "C4N-6";
    repo = "solitaire";
    tag = "v${version}";
    hash = "sha256-314zG4G2YdZ2Zk76C3jPrzTuUFZGX0wkaiOYMAImMRU=";
  };

  nativeBuildInputs = [
    pkgs.cmake
  ];

  buildInputs = [
    pkgs.nlohmann_json
    pkgs.argparse
  ];

  meta = {
    description = "a solitaire game";
    homepage = "https://github.com/C4N-6/solitaire";
  };
}
