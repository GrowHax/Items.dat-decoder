#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <stdexcept>

struct ExtendBuffer {
	std::vector<uint8_t> data;
	size_t mempos = 0;

	ExtendBuffer() = default;
	ExtendBuffer(size_t size) : data(size), mempos(0) {}
	ExtendBuffer(const std::vector<uint8_t>& d) : data(d), mempos(0) {}

	uint8_t readU8() {
		if (mempos >= data.size()) throw std::out_of_range("readU8 out of range");
		return data[mempos++];
	}
	int8_t readI8() { return static_cast<int8_t>(readU8()); }

	uint16_t readU16() {
		if (mempos + 1 >= data.size()) throw std::out_of_range("readU16 out of range");
		uint16_t v = data[mempos] | (data[mempos+1] << 8);
		mempos += 2;
		return v;
	}
	int16_t readI16() { return static_cast<int16_t>(readU16()); }

	uint32_t readU32() {
		if (mempos + 3 >= data.size()) throw std::out_of_range("readU32 out of range");
		uint32_t v = data[mempos] | (data[mempos+1] << 8) | (data[mempos+2] << 16) | (data[mempos+3] << 24);
		mempos += 4;
		return v;
	}
	int32_t readI32() { return static_cast<int32_t>(readU32()); }

	void writeU8(uint8_t v) {
		if (mempos >= data.size()) data.resize(mempos+1);
		data[mempos++] = v;
	}
	void writeI8(int8_t v) { writeU8(static_cast<uint8_t>(v)); }

	void writeU16(uint16_t v) {
		if (mempos + 1 >= data.size()) data.resize(mempos+2);
		data[mempos++] = v & 0xFF;
		data[mempos++] = (v >> 8) & 0xFF;
	}
	void writeI16(int16_t v) { writeU16(static_cast<uint16_t>(v)); }

	void writeU32(uint32_t v) {
		if (mempos + 3 >= data.size()) data.resize(mempos+4);
		data[mempos++] = v & 0xFF;
		data[mempos++] = (v >> 8) & 0xFF;
		data[mempos++] = (v >> 16) & 0xFF;
		data[mempos++] = (v >> 24) & 0xFF;
	}
	void writeI32(int32_t v) { writeU32(static_cast<uint32_t>(v)); }
};

struct ItemDefinition {
	int32_t id = 0;
	uint16_t flags = 0;
	uint8_t type = 0;
	uint8_t materialType = 0;

	std::string name;
	std::string texture;
	int32_t textureHash = 0;
	uint8_t visualEffectType = 0;
	int32_t cookingTime = 0;

	uint8_t textureX = 0;
	uint8_t textureY = 0;
	uint8_t storageType = 0;
	uint8_t isStripeyWallpaper = 0;
	uint8_t collisionType = 0;
	uint8_t breakHits = 0;

	int32_t resetStateAfter = 0;
	uint8_t bodyPartType = 0;
	int16_t rarity = 0;
	uint8_t maxAmount = 0;

	std::string extraFile;
	int32_t extraFileHash = 0;
	int32_t audioVolume = 0;

	std::string petName, petPrefix, petSuffix, petAbility;

	uint8_t seedBase = 0, seedOverlay = 0, treeBase = 0, treeLeaves = 0;
	int32_t seedColor = 0, seedOverlayColor = 0, ingredient = 0, growTime = 0;

	int32_t fxFlags = 0;

	std::string extraOptions, texture2, extraOptions2;

	int32_t unknownInt1 = 0, unknownInt2 = 0, flags2 = 0;

	std::vector<uint8_t> extraBytes; // 60 bytes

	int32_t tileRange = 0;
	int32_t vaultCapacity = 0;

	// version >= 11
	std::string punchOptions;
	int32_t flags3 = 0;
	std::vector<uint8_t> bodyPart; // 9 bytes
	int32_t lightRange = 0;
	int32_t unknownInt3 = 0;
	uint8_t canSit = 0;
	int32_t playerOffsetX = 0, playerOffsetY = 0;
	int32_t chairTextureX = 0, chairTextureY = 0;
	int32_t chairLegOffsetX = 0, chairLegOffsetY = 0;
	std::string chairTexture;
	std::string itemRenderer;
	int32_t extraFlags1 = 0;
	int32_t itemRendererHash = 0;
	std::vector<uint8_t> unknownBytes2; // 9 bytes
	int16_t unknownShort1 = 0;
	std::string info;
	std::vector<uint16_t> recipe; // 2 entries
};

struct ItemsDatMeta {
	std::unordered_map<int32_t, ItemDefinition> items;
	int32_t itemCount = 0;
	int16_t version = 0;
};

class ItemsDat {
public:
	ExtendBuffer buffer;
	std::string key = "PBG892FXX982ABC*";
	ItemsDatMeta meta;

	ItemsDat(const std::vector<uint8_t>& data) : buffer(data) {}
	ItemsDat(size_t size) : buffer(size) {}

	int getWriteSize() {
		int size = 130 * static_cast<int>(meta.items.size());
		for (auto &p : meta.items) {
			ItemDefinition &item = p.second;
			// string fields
			auto addStr = [&](const std::string &s){ size += static_cast<int>(s.size()) + 2; };
			addStr(item.name);
			addStr(item.texture);
			addStr(item.extraFile);
			addStr(item.petName);
			addStr(item.petPrefix);
			addStr(item.petSuffix);
			addStr(item.petAbility);
			addStr(item.extraOptions);
			addStr(item.texture2);
			addStr(item.extraOptions2);
			addStr(item.punchOptions);
			addStr(item.chairTexture);
			addStr(item.itemRenderer);
			addStr(item.info);

			// arrays
			size += static_cast<int>(item.extraBytes.size());
			size += static_cast<int>(item.bodyPart.size());
			size += static_cast<int>(item.unknownBytes2.size());
		}
		return size + 4 + 2;
	}

	std::string readString(bool encoded=false, int id=0) {
		int16_t len = buffer.readI16();
		std::string s;
		s.reserve(len);
		for (int i=0;i<len;i++){
			uint8_t c = buffer.readU8();
			if (encoded) c = c ^ static_cast<uint8_t>(key[(id + i) % key.size()]);
			s.push_back(static_cast<char>(c));
		}
		return s;
	}

	void writeString(const std::string &s, int id=0, bool encoded=false) {
		buffer.writeI16(static_cast<int16_t>(s.size()));
		for (size_t i=0;i<s.size();i++){
			uint8_t c = static_cast<uint8_t>(s[i]);
			if (encoded) c = c ^ static_cast<uint8_t>(key[(id + i) % key.size()]);
			buffer.writeU8(c);
		}
	}

	void decode() {
		buffer.mempos = 0;
		meta.version = buffer.readI16();
		meta.itemCount = buffer.readI32();
		for (int i=0;i<meta.itemCount;i++){
			ItemDefinition item;
			item.id = buffer.readI32();
			item.flags = buffer.readU16();
			item.type = buffer.readU8();
			item.materialType = buffer.readU8();

			item.name = readString(true, item.id);
			item.texture = readString(false, item.id);

			item.textureHash = buffer.readI32();
			item.visualEffectType = buffer.readU8();
			item.cookingTime = buffer.readI32();

			item.textureX = buffer.readU8();
			item.textureY = buffer.readU8();
			item.storageType = buffer.readU8();
			item.isStripeyWallpaper = buffer.readU8();
			item.collisionType = buffer.readU8();
			item.breakHits = buffer.readU8() / 6;

			item.resetStateAfter = buffer.readI32();
			item.bodyPartType = buffer.readU8();
			item.rarity = buffer.readI16();
			item.maxAmount = buffer.readU8();

			item.extraFile = readString(false, item.id);
			item.extraFileHash = buffer.readI32();
			item.audioVolume = buffer.readI32();

			item.petName = readString(false, item.id);
			item.petPrefix = readString(false, item.id);
			item.petSuffix = readString(false, item.id);
			item.petAbility = readString(false, item.id);

			item.seedBase = buffer.readU8();
			item.seedOverlay = buffer.readU8();
			item.treeBase = buffer.readU8();
			item.treeLeaves = buffer.readU8();

			item.seedColor = buffer.readI32();
			item.seedOverlayColor = buffer.readI32();
			item.ingredient = buffer.readI32();
			item.growTime = buffer.readI32();

			item.fxFlags = buffer.readI32();

			item.extraOptions = readString(false, item.id);
			item.texture2 = readString(false, item.id);
			item.extraOptions2 = readString(false, item.id);

			item.unknownInt1 = buffer.readI32();
			item.unknownInt2 = buffer.readI32();
			item.flags2 = buffer.readI32();

			// extraBytes: copy 60 bytes
			item.extraBytes.resize(60);
			for (int k=0;k<60;k++) item.extraBytes[k] = buffer.readU8();

			item.tileRange = buffer.readI32();
			item.vaultCapacity = buffer.readI32();

			if (meta.version >= 11) {
				item.punchOptions = readString(false, item.id);
				if (meta.version >= 12) {
					item.flags3 = buffer.readI32();
					item.bodyPart.resize(9);
					for (int k=0;k<9;k++) item.bodyPart[k] = buffer.readU8();
				}
				if (meta.version >= 13) item.lightRange = buffer.readI32();
				if (meta.version >= 14) item.unknownInt3 = buffer.readI32();
				if (meta.version >= 15) {
					item.canSit = buffer.readU8();
					item.playerOffsetX = buffer.readI32();
					item.playerOffsetY = buffer.readI32();

					item.chairTextureX = buffer.readI32();
					item.chairTextureY = buffer.readI32();

					item.chairLegOffsetX = buffer.readI32();
					item.chairLegOffsetY = buffer.readI32();

					item.chairTexture = readString(false, item.id);
				}
				if (meta.version >= 16) item.itemRenderer = readString(false, item.id);
				if (meta.version >= 17) item.extraFlags1 = buffer.readI32();
				if (meta.version >= 18) item.itemRendererHash = buffer.readI32();
				if (meta.version >= 19) {
					item.unknownBytes2.resize(9);
					for (int k=0;k<9;k++) item.unknownBytes2[k] = buffer.readU8();
				}
				if (meta.version >= 21) item.unknownShort1 = buffer.readI16();
				if (meta.version >= 22) item.info = readString(false, item.id);
				if (meta.version >= 23) {
					item.recipe.resize(2);
					for (int r=0;r<2;r++) {
						uint16_t data = buffer.readU16();
						if (data) item.recipe[r] = data;
					}
				}
			}

			meta.items[item.id] = std::move(item);
		}
	}

	void encode() {
		buffer.mempos = 0;
		int size = getWriteSize();
		buffer = ExtendBuffer(size);
		buffer.writeI16(meta.version);
		buffer.writeI32(static_cast<int32_t>(meta.items.size()));

		for (auto &p : meta.items) {
			ItemDefinition &item = p.second;
			buffer.writeI32(item.id);
			buffer.writeU16(item.flags);
			buffer.writeU8(item.type);
			buffer.writeU8(item.materialType);

			writeString(item.name, item.id, true);
			writeString(item.texture, item.id, false);

			buffer.writeI32(item.textureHash);
			buffer.writeU8(item.visualEffectType);
			buffer.writeI32(item.cookingTime);

			buffer.writeU8(item.textureX);
			buffer.writeU8(item.textureY);
			buffer.writeU8(item.storageType);
			buffer.writeU8(item.isStripeyWallpaper);
			buffer.writeU8(item.collisionType);
			buffer.writeU8(item.breakHits * 6);

			buffer.writeI32(item.resetStateAfter);
			buffer.writeU8(item.bodyPartType);
			buffer.writeI16(item.rarity);
			buffer.writeU8(item.maxAmount);

			writeString(item.extraFile, item.id, false);
			buffer.writeI32(item.extraFileHash);
			buffer.writeI32(item.audioVolume);

			writeString(item.petName, item.id, false);
			writeString(item.petPrefix, item.id, false);
			writeString(item.petSuffix, item.id, false);
			writeString(item.petAbility, item.id, false);

			buffer.writeU8(item.seedBase);
			buffer.writeU8(item.seedOverlay);
			buffer.writeU8(item.treeBase);
			buffer.writeU8(item.treeLeaves);

			buffer.writeI32(item.seedColor);
			buffer.writeI32(item.seedOverlayColor);
			buffer.writeI32(item.ingredient);
			buffer.writeI32(item.growTime);

			buffer.writeI32(item.fxFlags);

			writeString(item.extraOptions, item.id, false);
			writeString(item.texture2, item.id, false);
			writeString(item.extraOptions2, item.id, false);

			buffer.writeI32(item.unknownInt1);
			buffer.writeI32(item.unknownInt2);
			buffer.writeI32(item.flags2);

			for (auto b : item.extraBytes) buffer.writeU8(b);

			buffer.writeI32(item.tileRange);
			buffer.writeI32(item.vaultCapacity);

			if (meta.version >= 11) {
				writeString(item.punchOptions, item.id, false);
				if (meta.version >= 12) {
					buffer.writeI32(item.flags3);
					for (auto b : item.bodyPart) buffer.writeU8(b);
				}
				if (meta.version >= 13) buffer.writeI32(item.lightRange);
				if (meta.version >= 14) buffer.writeI32(item.unknownInt3);
				if (meta.version >= 15) {
					buffer.writeU8(item.canSit);
					buffer.writeI32(item.playerOffsetX);
					buffer.writeI32(item.playerOffsetY);

					buffer.writeI32(item.chairTextureX);
					buffer.writeI32(item.chairTextureY);

					buffer.writeI32(item.chairLegOffsetX);
					buffer.writeI32(item.chairLegOffsetY);

					writeString(item.chairTexture, item.id, false);
				}
				if (meta.version >= 16) writeString(item.itemRenderer, item.id, false);
				if (meta.version >= 17) buffer.writeI32(item.extraFlags1);
				if (meta.version >= 18) buffer.writeI32(item.itemRendererHash);
				if (meta.version >= 19) for (auto b : item.unknownBytes2) buffer.writeU8(b);
				if (meta.version >= 21) buffer.writeI16(item.unknownShort1);
				if (meta.version >= 22) writeString(item.info, item.id, false);
				if (meta.version >= 23) {
					buffer.writeU16(item.recipe.size()>0?item.recipe[0]:0);
					buffer.writeU16(item.recipe.size()>1?item.recipe[1]:0);
				}
			}
		}
	}
};

static std::vector<uint8_t> readFile(const std::string &path) {
	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	if (!ifs) throw std::runtime_error("Failed to open file: " + path);
	std::ifstream::pos_type pos = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<uint8_t> data(static_cast<size_t>(pos));
	if (!data.empty()) ifs.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(pos));
	return data;
}

static void writeFile(const std::string &path, const std::vector<uint8_t> &data) {
	std::ofstream ofs(path, std::ios::binary);
	if (!ofs) throw std::runtime_error("Failed to open file for writing: " + path);
	if (!data.empty()) ofs.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
}

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
			// Build JSON manually
			std::ostringstream out;
			out << "{\n";
			out << "  \"version\": " << items.meta.version << ",\n";
			out << "  \"itemCount\": " << items.meta.items.size() << ",\n";
			out << "  \"items\": [\n";
			bool firstItem = true;
			for (const auto &p : items.meta.items) {
				if (!firstItem) out << ",\n";
				firstItem = false;
				const ItemDefinition &it = p.second;
				out << "    {\n";
				out << "      \"id\": " << it.id << ",\n";
				out << "      \"name\": \"";
				// escape name
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
			// Example: write back to disk as round-trip test
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

