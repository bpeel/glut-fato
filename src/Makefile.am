AM_CPPFLAGS = \
	$(GL_CFLAGS) \
	$(GLEW_CFLAGS) \
	$(FATO_EXTRA_CFLAGS) \
	$(NULL)

bin_PROGRAMS = \
	fato-mipmap-fb \
	fato-triangle \
	$(NULL)

fato_triangle_SOURCES = \
	fato-triangle.c \
	$(NULL)

fato_triangle_LDADD = \
	$(GL_LIBS) \
	$(GLEW_LIBS) \
	$(NOTBIT_EXTRA_LIBS) \
	$(NULL)

fato_mipmap_fb_SOURCES = \
	fato-mipmap-fb.c \
	$(NULL)

fato_mipmap_fb_LDADD = \
	$(GL_LIBS) \
	$(GLEW_LIBS) \
	$(NOTBIT_EXTRA_LIBS) \
	$(NULL)
