LIBRARIES := caffe glog gflags protobuf leveldb snappy lmdb boost_system boost_thread hdf5_hl hdf5 opencv_core opencv_highgui opencv_imgproc pthread unwind openblas
LIBRARY_DIRS := 3rdparty/lib/
INCLUDE_DIRS := 3rdparty/include/ /home/youzhi.zou/neural/apsara/

CXX := /usr/bin/g++

LD_FLAGS += $(foreach library,$(LIBRARIES),-l$(library)) \
            $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))

COMMON_FLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

COMMON_FLAGS += -DCPU_ONLY

CXX_SRC := $(shell find src/ -name "*.cpp")

OUTPUT := -o build/run_dnn

all:
	$(CXX) $(COMMON_FLAGS) $(LD_FLAGS) $(CXX_SRC) $(OUTPUT)
