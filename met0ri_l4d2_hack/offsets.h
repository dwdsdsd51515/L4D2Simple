#pragma once

namespace offsets
{
	uintptr_t clientmode = 0;
	uintptr_t cinput = 0;
	uintptr_t clientstate = 0;

	uintptr_t find_pattern(HMODULE module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return (uintptr_t)&scanBytes[i];
			}
		}
		return 0;
	}

	void initialize()
	{
		auto client_dll = GetModuleHandleA("client.dll");
		auto engine_dll = GetModuleHandleA("engine.dll");

		cinput = find_pattern(client_dll, "8B 55 10 8B 0D ? ? ? ? 8B 01 8B 40 0C");
		cinput = *(uintptr_t*)(cinput + 5);

		clientstate = find_pattern(engine_dll, "E8 ? ? ? ? 8B 88 ? ? ? ? 41");
		clientstate = follow_rel32<uintptr_t>(clientstate, 1);

		/*clientmode = find_pattern(client_dll, "8B 0D ? ? ? ? 8B 01 8B 90 ? ? ? ? FF D2 8B 04 85 ? ? ? ? 8B 10 8B C8 8B 42 74");
		printf("clientmode_1: 0x%X\n", clientmode);
		clientmode = *(uintptr_t*)(clientmode + 2);
		printf("clientmode_2: 0x%X\n", clientmode);*/
	}
}