linpack_LTLIBRARIES             = libptflinpack.la
linpackdir = ${plugindir}/linpack/

libptflinpack_la_CXXFLAGS = ${autotune_plugin_base_cxxflags} \
                                   -I$(top_srcdir)/autotune/plugins/linpack/include

libptflinpack_la_SOURCES  = autotune/plugins/linpack/src/Linpack.cc
libptflinpack_la_LDFLAGS  = ${autotune_plugin_base_ldflags} -version-info 1:0:0
