# Evolution

This document describes how this project should evolve.
It should help users of this project to decide whether or not they can expect this project to fulfill their requirements right now or in the future.
It should not include any implementation details, but only an informal description of goals and methods to achieve them.
This document may also change as the project evolves, but changes should always be minor and infrequent, since this document should describe the long term vision.

## Goals

### Primary

The primary goal of this project is to provide a way to generate JavaScript code coverage reports from tests run with `Jasmine` in the `Grunt` build-system.

### Secondary

The primary goal should be achieved in way as transparent as possible, which means that the use of this project should not influence the way other plugins work.

## Methods

### Versioning

This project tries to follow the [semantic versioning](http://semver.org/) principle.
This means that until the major version 1 is released, the public API may also change, i.e. non-backwards-compatible changes may be introduced, but they will only happen if it is ultimately necessary.
It is yet to be defined when version 1 will be released and which features it will include.

### Testing

To ensure trust in the implementation of the project's features, they should be tested by writing automated tests.
This mechanism should also help detect backwards-incompatible changes, as tests will fail in this case.

### Independence

This project should have as few as possible dependencies, both direct - as declared dependencies - and indirect - as special handling in code for other projects used in parallel.
By keeping the dependencies as lean as possible, managing evolution becomes easier as breaking changes in dependencies are less frequent.
