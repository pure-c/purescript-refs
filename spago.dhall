{ name = "refs"
, dependencies = [ "prelude", "effect", "assert" ]
, packages = ../pure-c/package-sets/packages.dhall
, sources = [ "src/**/*.purs", "test/**/*.purs" ]
}
