AM_CPPFLAGS = \
	$(GL_CFLAGS) \
	$(EPOXY_CFLAGS) \
	$(FATO_EXTRA_CFLAGS) \
	$(NULL)

bin_PROGRAMS = \
	fato-ffs \
	fato-mipmap-fb \
	fato-texture-array \
	fato-triangle \
	fato-pbo \
	$(NULL)

fato_ffs_SOURCES = \
	fato-ffs.c \
	fato-util.c \
	$(NULL)

fato_ffs_LDADD = \
	$(GL_LIBS) \
	$(EPOXY_LIBS) \
	$(FATO_EXTRA_LIBS) \
	$(NULL)

fato_triangle_SOURCES = \
	fato-triangle.c \
	$(NULL)

fato_triangle_LDADD = \
	$(GL_LIBS) \
	$(EPOXY_LIBS) \
	$(FATO_EXTRA_LIBS) \
	$(NULL)

fato_pbo_SOURCES = \
	fato-pbo.c \
	$(NULL)

fato_pbo_LDADD = \
	$(GL_LIBS) \
	$(EPOXY_LIBS) \
	$(FATO_EXTRA_LIBS) \
	$(NULL)

fato_mipmap_fb_SOURCES = \
	fato-mipmap-fb.c \
	$(NULL)

fato_mipmap_fb_LDADD = \
	$(GL_LIBS) \
	$(EPOXY_LIBS) \
	$(FATO_EXTRA_LIBS) \
	$(NULL)

fato_texture_array_SOURCES = \
	fato-texture-array.c \
	fato-util.c \
	$(NULL)

fato_texture_array_LDADD = \
	$(GL_LIBS) \
	$(EPOXY_LIBS) \
	$(FATO_EXTRA_LIBS) \
	$(NULL)
