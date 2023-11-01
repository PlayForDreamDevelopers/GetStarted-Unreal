#ifndef YVRLIB_COMPILE_H
#define YVRLIB_COMPILE_H

#if defined(YVRLIB_ENABLE_EXPORT)
#define YVRLIB_EXPORT __attribute__((__visibility__("default")))
#else
#define YVRLIB_EXPORT
#endif

#define YVRLIB_DEPRECATED(fn) fn __attribute__((deprecated))

#if defined(__aarch64__)
#define YVRLIB_64_BIT
#else
#define YVRLIB_32_BIT
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
#define YVRLIB_STATIC_ASSERT(exp) static_assert(exp, #exp)
#endif

#if !defined(YVRLIB_STATIC_ASSERT) && defined(__clang__)
#if __has_feature(cxx_static_assert) || __has_extension(cxx_static_assert)
#define YVRLIB_STATIC_ASSERT(exp) static_assert(exp)
#endif
#endif

#if !defined(YVRLIB_STATIC_ASSERT)
#if defined(__COUNTER__)
#define YVRLIB_STATIC_ASSERT(exp) YVRLIB_STATIC_ASSERT_ID(exp, __COUNTER__)
#else
#define YVRLIB_STATIC_ASSERT(exp) YVRLIB_STATIC_ASSERT_ID(exp, __LINE__)
#endif
#define YVRLIB_STATIC_ASSERT_ID(exp, id) YVRLIB_STATIC_ASSERT_ID_EXPANDED(exp, id)
#define YVRLIB_STATIC_ASSERT_ID_EXPANDED(exp, id) typedef char assert_failed_##id[(exp) ? 1 : -1]
#endif

#if defined(__COUNTER__)
#define YVRLIB_PADDING(bytes) YVRLIB_PADDING_ID(bytes, __COUNTER__)
#else
#define YVRLIB_PADDING(bytes) YVRLIB_PADDING_ID(bytes, __LINE__)
#endif
#define YVRLIB_PADDING_ID(bytes, id) YVRLIB_PADDING_ID_EXPANDED(bytes, id)
#define YVRLIB_PADDING_ID_EXPANDED(bytes, id) unsigned char dead##id[(bytes)];

#define YVRLIB_ASSERT_TYPE_SIZE(type, bytes) YVRLIB_STATIC_ASSERT(sizeof(type) == (bytes))

#if defined(YVRLIB_64_BIT)
#define YVRLIB_PADDING_32_BIT(bytes)
#if defined(__COUNTER__)
#define YVRLIB_PADDING_64_BIT(bytes) YVRLIB_PADDING_ID(bytes, __COUNTER__)
#else
#define YVRLIB_PADDING_64_BIT(bytes) YVRLIB_PADDING_ID(bytes, __LINE__)
#endif
#define YVRLIB_ASSERT_TYPE_SIZE_32_BIT(type, bytes) YVRLIB_STATIC_ASSERT(true)
#define YVRLIB_ASSERT_TYPE_SIZE_64_BIT(type, bytes) YVRLIB_STATIC_ASSERT(sizeof(type) == (bytes))
#else
#define YVRLIB_ASSERT_TYPE_SIZE(type, bytes) YVRLIB_STATIC_ASSERT(sizeof(type) == (bytes))
#if defined(__COUNTER__)
#define YVRLIB_PADDING_32_BIT(bytes) YVRLIB_PADDING_ID(bytes, __COUNTER__)
#else
#define YVRLIB_PADDING_32_BIT(bytes) YVRLIB_PADDING_ID(bytes, __LINE__)
#endif
#define YVRLIB_PADDING_64_BIT(bytes)
#define YVRLIB_ASSERT_TYPE_SIZE_32_BIT(type, bytes) YVRLIB_STATIC_ASSERT(sizeof(type) == (bytes))
#define YVRLIB_ASSERT_TYPE_SIZE_64_BIT(type, bytes) YVRLIB_STATIC_ASSERT(true)
#endif

#if !defined(YVRLIB_ALIGN)
#define YVRLIB_ALIGN(value, boundary) ((value + boundary - 1) & (~(boundary - 1)))
#endif

#endif
