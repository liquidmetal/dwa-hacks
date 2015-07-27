#!/usr/local/bin/tcsh

setenv LD_LIBRARY_PATH /rel/folio/openvdb/openvdb-2.3.0-6/lib/openvdb/lib/gcc48_64/:${LD_LIBRARY_PATH}
/work/td/usinha/dwa-hacks-master-ughufran/pipeline/vdb_map_converter $*
