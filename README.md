# MAPF
Experiments in Multi-Agent Pathfinding

## Getting started

### Clone repository

```sh
git clone git@github.com:robotics-laboratory/mapf.git
```

### Docker 

#### Build and Run container

```sh
# Build and Run
docker compose build mapf
docker compose up -d mapf

# Attach to shell
docker exec -it mapf-${username} zsh

# Stop container
docker stop mapf-${username}
```

### Bazel

#### Build and Run target

```sh
# Build 
bazel build //...

# Run 
bazel run //... -- ${binary options}

# Test
bazel test --test_output=all //...
```

## Running solver

### Show information about solver binary options

```sh
bazel run //solver:main -- --help
```

### List available solver names

```sh
bazel run //solver:main -- --list-solvers
```

Solver input is a file with text formated [MAPFProblem](models/proto/models.proto#L22) protobuf message. [See](data/mapf_problem_1.pb.txt) example of file structure.

Solver output is a file with text formated [MAPFSolution](models/proto/models.proto#L27) protobuf message.

Use paths, relative to MODULE directory.

### Solver run command example

```sh
bazel run //solver:main -- -s bfs_solver -i data/mapf_problem_1.pb.txt -o data/mapf_solution_1.pb.txt
```

## Running simulator

### Show information about simulator binary options

```sh
bazel run //simulator:main -- --help
```

### List available solver names

```sh
bazel run //simulator:main -- --list-solvers
```

Simulator input is a file with text formated [MAPFProblem](models/proto/models.proto#L22) protobuf message. [See](data/mapf_problem_1.pb.txt) example of file structure.

Simulator output is a MCAP file with serialized [Scene](scene/proto/scene.proto) protobuf message.

Use paths, relative to MODULE directory.

### Simulator run command example

```sh
bazel run //simulator:main -- -s bfs_solver -i data/mapf_problem_1.pb.txt -o data/scene.mcap
```

## Repository contents

Guide to packages:

* __geom__: Package with geometry primitives (`Vec2`, floating point number comparators, etc.). See __geom/tests__ for usage examples, __geom/proto__ for available proto-definitions.

* __graph__: Package with graph primitives (`Node`, `Edge`, `Endpoints`, etc.). See __graph/test__ for usage examples, __graph/proto__ for available proto-definitions.

* __models__: Package with MAPF-problem specific primitives (`AgentState`, `AgentTask`, `AgentPath`, `MAPFProblem`, `MAPFSolution`, etc.). See __models/test__ for usage examples, __models/proto__ for available proto-definitions.

* __scene__: Package with scene primitive, being recorded during simulation. See __models/proto__ for available proto-definitions.

* __simulator__: Package with simulator primitives. The simulator is implemented using a single-threaded [actor model](https://en.wikipedia.org/wiki/Actor_model).
    
    * __simulator/acotrs__: Available actors declarations, `Actor` interface is defined [here](simulator/actors/include/actors/actor.h).

    * __simulator/context__: Global simulation context, holding current timestamp and `EventBus`, which is an object, that encapsulates interaction between different actors.

    * __simulator/event__: Event primitives, which encapsulates messages handling in runtime.

    * __simulator/launcher__: Simulation launcher, running event loop.

    * __simulator/main__: Simulator binary. See [CLI-options](simulator/main/src/main.cpp#L24) to determine available running modes.

    * __simulator/messages__: Available messages declarations. `Message` interface is defined [here](simulator/messages/include/messages/message.h).

* __solver__: Package with available MAPF-problem solvers. 

    * __solver/solvers__: Available solvers declarations, `Solver` interface is defined [here](solver/solvers/include/solvers/solver_base.h).

    * __solver/factory__: Factory with registered solvers. After registering solver in [factory constructor](solver/factory/src/solver_factory.cpp#L11), it automatically becomes available in CLI-options.

    * __solver/main__: Sover binary. See [CLI-options](solver/main/src/main.cpp#L17) to determine available running modes.