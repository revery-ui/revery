source /opt/rh/llvm-toolset-7.0/enable
clang -v

# Workaround for: https://github.com/esy/esy/issues/1227
# Concurrent fetch seems to cause hang on Docker in Azure Pipelines..
export ESY__BUILD_CONCURRENCY=1
export ESY__FETCH_CONCURRENCY=1

esy install
esy build
