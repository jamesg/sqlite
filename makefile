first:	libsqlite.a

CPPFLAGS=-std=c++0x -O3 -Iinclude/ -I${JSON_LIBRARY}/include/
LIBS=-lsqlite3 -lboost_regex

OBJDIR=objs

# If a JSON library path was not provided, we must define one.
ifndef JSON_LIBRARY
	JSON_LIBRARY=json
endif

OBJS=\
	${OBJDIR}/detail/insert_query_string.o \
	${OBJDIR}/detail/last_insert_rowid.o \
	${OBJDIR}/bind_values.o \
	${OBJDIR}/connection.o \
	${OBJDIR}/get_column.o \
	${OBJDIR}/row.o \
	${OBJDIR}/step.o \
	${OBJDIR}/transaction.o

clean:
	rm -f ${OBJS}

${JSON_LIBRARY}/libjson.a:
	cd ${JSON_LIBRARY} && make libjson.a

libsqlite.a:	${OBJS} ${JSON_LIBRARY}/libjson.a
	ar rvs libsqlite.a ${OBJS}

bin/main:	${JSON_LIBRARY}/libjson.a libsqlite.a src/main.cpp
	${CXX} ${CPPFLAGS} src/main.cpp ${JSON_LIBRARY}/libjson.a libsqlite.a ${LIBS} -o $@

${OBJDIR}/%.o : src/%.cpp
	${CXX} ${CPPFLAGS} -Isrc $< -c -o $@

