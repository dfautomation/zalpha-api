# zalpha-api

## Introduction

The %Zalpha AGV exposes a set of API through its API server when the "Development Mode" app is run.

The API server uses the [ZeroMQ](http://zeromq.org/) protocol, and communicates on the [Request-Reply](http://zguide.zeromq.org/page:all#Ask-and-Ye-Shall-Receive) mode. It binds its ZMQ_REP (reply) socket to port 17167, waits for a request in a loop, and responds each time with a reply.

This client library provides the C++ class to ease the access to the %Zalpha API.


## Installation

This API client library can be used on the Linux and Windows platform.

The zalpha-api depends on the ZeroMQ library, therefore the ZeroMQ library must be downloaded and installed prior to using this library.

For Linux (Ubuntu, Debian), use apt-get to install ZeroMQ:

    sudo apt-get install libzmq5

or for an older version of Ubuntu (14.04), use:

    sudo apt-get install libzmq3

For Linux (RHEL, centOS), use yum to install ZeroMQ:

    sudo yum install zeromq

For Windows, the installer can be downloaded from the ZeroMQ [website](http://zeromq.org/distro:microsoft-windows).


### Build

The pre-built binaries is provided for Linux and can be downloaded on the [release](http://github.com/dfautomation/zalpha-api/releases) page.

For other platforms, you may build the binary from the source tarball. You need to install [CMake](http://cmake.org) and the [libzmq3-dev](https://packages.ubuntu.com/search?keywords=libzmq3-dev) library in order to build from source.

Download the tarball from the [release](http://github.com/dfautomation/zalpha-api/releases) page or use the latest development code in the [master](http://github.com/dfautomation/zalpha-api/tarball/master) branch.

Extract the tarball into a folder. Under the folder, run the following commands:

~~~{.sh}
mkdir build && cd build
cmake ..
make
make doc
~~~


## Usage

Please refer to the zalpha_api namespace and the zalpha_api::Zalpha class for the API details.


## Project Page

This project is hosted at [Github](http://github.com/dfautomation/zalpha-api).


## License

zalpha-api is relased under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and
limitations under the License.

%Zalpha is a trademark of [DF Automation & Robotics Sdn. Bhd.](http://www.dfautomation.com/)
