#pragma once

#ifdef LIBASYNC
#ifdef _WIN32
#define LIBASYNC_EXPORT __declspec(dllexport)
#elif __GNUC__ >= 4
#define LIBASYNC_EXPORT __attribute__((visibility("default")))
#else
#define LIBASYNC_EXPORT
#endif
#else
#ifdef _WIN32
#define LIBASYNC_EXPORT __declspec(dllimport)
#else
#define LIBASYNC_EXPORT
#endif
#endif

namespace libasync
{
	using context_t = uint32_t;

	LIBASYNC_EXPORT context_t connect(uint32_t N);
	LIBASYNC_EXPORT bool receive(context_t, const char* buff, size_t buff_size);
	LIBASYNC_EXPORT bool disconnect(context_t);
}