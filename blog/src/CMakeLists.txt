SET(SOURCES
  author.cpp
  person.cpp
  main.cpp
  post.cpp)

ADD_EXECUTABLE(mblog ${SOURCES})

TARGET_LINK_LIBRARIES(mblog
  matador-utils
  matador-object
  matador-sql
  matador-orm
  matador-sqlite
  ${CMAKE_DL_LIBS}
  ${DB_LIBRARIES}
)

TARGET_INCLUDE_DIRECTORIES(mblog PUBLIC ${PROJECT_SOURCE_DIR}/blog/include)