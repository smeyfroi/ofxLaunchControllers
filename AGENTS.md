# AGENTS.md (ofxLaunchControllers)

## Build / lint / test
- Build example app (mac/linux): `make -C example_launchcontrol` (uses `example_launchcontrol/config.make` + OF make system).
- Clean example build: `make -C example_launchcontrol clean`.
- Xcode: open `example_launchcontrol/example_launchcontrol.xcodeproj` and build the “Debug/Release” schemes.
- CI-style addon build (requires full OF checkout): `openFrameworks/scripts/ci/addons/build.sh` (see `.travis.yml`).
- Lint/tests: none included in-repo; validate changes by building (and optionally running) `example_launchcontrol`.
- Single test: N/A (no test runner); use the example app as the smoke test.

## Code style
- Language: C++ (openFrameworks + ofxMidi). Headers use `#pragma once`.
- Includes: local header first, then OF headers (`ofMain.h`), then STL (`<atomic>`); keep includes minimal.
- Naming: types/classes `ofx*`; methods `lowerCamelCase`; bool members typically `bSomething`; raw pointers often `pThing`/`pParam*`.
- Formatting: follow existing file style (tabs common in `.cpp`, braces on same line); avoid reformatting whole files.
- Error handling: prefer `ofLogError()`/`ofLogNotice()` + early-return; ignore invalid indices rather than throwing.
- Threading/MIDI callbacks: avoid allocations; use atomics/flags (pattern: buffer + `std::atomic` + update loop).

## Editor rules
- No `.cursor/rules/`, `.cursorrules`, or `.github/copilot-instructions.md` found in this repo.
