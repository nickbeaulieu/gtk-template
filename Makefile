.PHONY: bootstrap
bootstrap:
	./scripts/bootstrap.sh

.PHONY: build
build:
	./scripts/build.sh

.PHONY: run
run:
	./build/gtk-template