#include "include/ItemsDat.h"
#include "include/file_io.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

int main(int argc, char** argv) {
	try {
		std::string inPath = "items.dat";
		// If first arg exists and does not start with '-', treat it as input path
		int ai = 1;
		if (argc > 1) {
			std::string first = argv[1];
			if (!first.empty() && first[0] != '-') {
				inPath = first;
				ai = 2;
			} else {
				ai = 1;
			}
		}

		auto data = readFile(inPath);
		if (data.empty()) {
			std::cout << "File is empty or not found: " << inPath << std::endl;
			return 1;
		}

		ItemsDat items(data);
		items.decode();

		std::cout << "Decoded items.dat" << std::endl;
		std::cout << "Version: " << items.meta.version << std::endl;
		std::cout << "Item count (meta): " << items.meta.itemCount << std::endl;
		std::cout << "Items read: " << items.meta.items.size() << std::endl;

		// If requested, output JSON representation
		bool wantJson = false;
		std::string jsonOutPath;
		for (int aj = ai; aj < argc; ++aj) {
			std::string a = argv[aj];
			if (a == "--json") {
				wantJson = true;
				// optional path follows if present and not starting with '-'
				if (aj + 1 < argc) {
					std::string maybe = argv[aj+1];
					if (!maybe.empty() && maybe[0] != '-') {
						jsonOutPath = maybe;
						++aj;
					} else {
						jsonOutPath = "items.json"; // default
					}
				} else {
					jsonOutPath = "items.json";
				}
			} else if (a == "--json-stdout") {
				wantJson = true;
				jsonOutPath = "-";
			}
		}

		if (wantJson) {
			std::ostringstream out;
			out << "{\n";
			out << "  \"version\": " << items.meta.version << ",\n";
			out << "  \"itemCount\": " << items.meta.items.size() << ",\n";
			out << "  \"items\": [\n";
			bool firstItem = true;

			// Collect and sort items by id to ensure ordered output
			std::vector<const ItemDefinition*> sortedItems;
			sortedItems.reserve(items.meta.items.size());
			for (const auto &p : items.meta.items) sortedItems.push_back(&p.second);
			std::sort(sortedItems.begin(), sortedItems.end(), [](const ItemDefinition* a, const ItemDefinition* b){
				return a->id < b->id;
			});

			for (const auto *pit : sortedItems) {
				if (!firstItem) out << ",\n";
				firstItem = false;
				const ItemDefinition &it = *pit;
				out << "    {\n";
				out << "      \"id\": " << it.id << ",\n";
				out << "      \"name\": \"";

				for (char c : it.name) {
					switch (c) {
						case '\\': out << "\\\\"; break;
						case '"': out << "\\\""; break;
						case '\n': out << "\\n"; break;
						case '\r': out << "\\r"; break;
						case '\t': out << "\\t"; break;
						default: out << c; break;
					}
				}
				out << "\",\n";
				out << "      \"texture\": \"";
				
				for (char c : it.texture) {
					switch (c) {
						case '\\': out << "\\\\"; break;
						case '"': out << "\\\""; break;
						case '\n': out << "\\n"; break;
						case '\r': out << "\\r"; break;
						case '\t': out << "\\t"; break;
						default: out << c; break;
					}
				}
				out << "\",\n";
				out << "      \"flags\": " << it.flags << ",\n";
				out << "      \"type\": " << static_cast<int>(it.type) << ",\n";
				out << "      \"materialType\": " << static_cast<int>(it.materialType) << "\n";
				out << "    }";
			}
			out << "\n  ]\n}\n";

			if (jsonOutPath.empty() || jsonOutPath == "-") {
				std::cout << out.str();
			} else {
				std::ofstream ofs(jsonOutPath, std::ios::binary);
				if (!ofs) throw std::runtime_error("Failed to open JSON output: " + jsonOutPath);
				std::string s = out.str();
				ofs.write(s.data(), static_cast<std::streamsize>(s.size()));
				ofs.close();
				std::cout << "Wrote JSON to " << jsonOutPath << std::endl;
			}
		} else {
			items.encode();
			writeFile("items_out.dat", items.buffer.data);
			std::cout << "Wrote items_out.dat (round-trip) size=" << items.buffer.data.size() << std::endl;
		}
	} catch (const std::exception &ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 2;
	}
	return 0;
}

