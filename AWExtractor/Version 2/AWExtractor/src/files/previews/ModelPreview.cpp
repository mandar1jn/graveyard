#include "ModelPreview.h"
#include <format>

namespace AWExtractor
{
	ModelPreview::ModelPreview(BinIndexFile* file) : PreviewFile(file)
	{
		version = er->ReadU32();
		if (version != 43)
			return;
		vertexBufferSize = er->ReadU32();
		positionOnlyVertexBufferSize = er->ReadU32();
		indexCount = er->ReadU32();
		indexSize = er->ReadU32();
		er->Skip(4); //unknown

		vertexBuffer = er->ReadBytes(vertexBufferSize);
		positionOnlyVertexBuffer = er->ReadBytes(positionOnlyVertexBufferSize);

		for (u32 i = 0; i < indexCount; i++)
		{
			if (indexSize == 2)
			{
				indexBuffer.push_back(er->ReadU16());
			}
			else
				throw "implement me";
		}

		boneCount = er->ReadU32();

		if (boneCount != 0)
		{
			bones = std::vector<Bone>();

			for (u32 i = 0; i < boneCount; i++)
			{
				Bone bone = {};

				bone.name = er->ReadString();

				for (u32 j = 0; j < 12; j++)
				{
					bone.bonePoses[j] = er->ReadFloat();
				}

				er->Skip(0x14); //unknown
			}

			er->Skip(0x10);
		}

		er->Skip(40);

		s64 offset = er->GetPosition() - file->offset;

		{
			boundBox = {};
			boundBox.min = std::vector<float>();
			boundBox.max = std::vector<float>();

			for (u8 i = 0; i < 3; i++)
			{
				boundBox.min.push_back(er->ReadFloat());
			}

			for (u8 i = 0; i < 3; i++)
			{
				boundBox.max.push_back(er->ReadFloat());
			}
		}

		lodCount = er->ReadU32();
		materialCount = er->ReadU32();
		materials = std::vector<Material>();

		for (u32 i = 0; i < materialCount; i++)
		{
			Material mat = {};

			er->Skip(4);

			mat.name = er->ReadString();
			mat.shaderName = er->ReadString();
			mat.sourcePath = er->ReadString();

			er->Skip(er->ReadU32() * 8);
			er->Skip(4 * 4);
			er->Skip(4);

			mat.attributeCount = er->ReadU32();

			mat.attributes = {};

			for (u32 i = 0; i < mat.attributeCount; i++)
			{
				Attribute attribute = Attribute();

				s64 offset = er->GetPosition() - file->offset;

				attribute.name = er->ReadString();

				attribute.type = (AttributeType)er->ReadU32();

				switch (attribute.type)
				{
				case FLOAT:
				{
					attribute.valueFloat = er->ReadFloat();
					//er->Skip(13);
					break;
				}
				case VEC2:
				{
					attribute.valueVec2[0] = er->ReadFloat();
					attribute.valueVec2[1] = er->ReadFloat();
					break;
				}
				case VEC3:
				{
					attribute.valueVec3[0] = er->ReadFloat();
					attribute.valueVec3[1] = er->ReadFloat();
					attribute.valueVec3[2] = er->ReadFloat();
					break;
				}
				case VEC4:
				{
					attribute.valueVec4[0] = er->ReadFloat();
					attribute.valueVec4[1] = er->ReadFloat();
					attribute.valueVec4[2] = er->ReadFloat();
					attribute.valueVec4[3] = er->ReadFloat();
					break;
				}
				case SAMPLER:
				{
					break;
				}
				case TEXTURE:
				{
					attribute.valueTexture = er->ReadString();
					break;
				}
				case BOOL:
				{
					attribute.valueBool = (er->ReadU32()) != 0;
					break;
				}
				default:
					throw "";
				}
				
				mat.attributes.push_back(attribute);
			}

			materials.push_back(mat);
		}

		er->Skip(er->ReadU32() * 4);
		er->Skip(4);
		er->Skip(er->ReadU32() * 4);

		meshCount = er->ReadU32();

		meshes = std::vector<Mesh>();

		for (u32 i = 0; i < 1/*meshCount*/; i++)
		{
			ReadMesh();
		}

	}
	void ModelPreview::ReadMesh()
	{
		Mesh mesh = {};

		u32 colorCount = 0;
		u32 uvCount = 0;

		mesh.lod = er->ReadU32();
		assert(mesh.lod < lodCount);
		mesh.vertexCount = er->ReadU32();
		mesh.faceCount = er->ReadU32();
		mesh.vertexOffset = er->ReadU32();
		mesh.positionOnlyVertexOffset = er->ReadU32();
		mesh.faceOffset = er->ReadU32();
		er->Skip(4);

		er->Skip(4 * 4);
		er->Skip(6 * 4);
		er->Skip(4);

		s64 position = er->GetPosition();
		mesh.vertexAttributeCount = er->ReadU8();

		for (u32 i = 0; i < mesh.vertexAttributeCount; i++)
		{
			VertexAttribute att = {};

			att.positionOnlyBuffer = er->ReadBool();

			u8 type = er->ReadU8();
			u8 dataType = er->ReadU8();

			if (type == 2)
			{
				att.type = VertexAttributeType::POSITION;
			}
			else if (type == 4)
			{
				att.type == VertexAttributeType::COLOR;
				colorCount++;
			}
			else if (type == 5)
			{
				if (dataType == 5)
				{
					if (dataType == 5)
					{
						att.type = VertexAttributeType::BONE_INDEX;
					}
					else
					{
						att.type = VertexAttributeType::BONE_WEIGHT;
					}
				}
			}
			else if (type == 7)
			{
				att.type = VertexAttributeType::TEX_COORD;
				uvCount++;
			}
			else if (type == 8)
			{
				att.type = VertexAttributeType::NORMAL;
			}
			else if (type == 13)
			{
				att.type = VertexAttributeType::BONE_INDEX;
			}

			er->Skip(1);

			mesh.vertexAttributes.push_back(att);
		}

		er->Skip(13);

		mesh.boneCount = er->ReadU32();

		if (mesh.boneCount != 0)
			throw "implement me!";

		if (colorCount != 0)
			throw "implement me!";
		if (uvCount != 0)
			throw "implement me!";

		u32 indexPosition = mesh.faceOffset;

		for (u32 i = 0; i < mesh.faceCount; i++)
		{
			mesh.faces.push_back(std::array<u32, 3>{indexBuffer[indexPosition++], indexBuffer[indexPosition++], indexBuffer[indexPosition++]});
		}

		meshes.push_back(mesh);
	}
	void ModelPreview::Draw()
	{
		ImGui::Text("Model");
		if (version == 43)
		{
			ImGui::Text("version: %i", version);
			ImGui::Text("vertexBufferSize: %i", vertexBufferSize);
			ImGui::Text("positionOnlyVertexBufferSize: %i", positionOnlyVertexBufferSize);
			ImGui::Text("indexCount: %i", indexCount);
			ImGui::Text("indexSize: %i", indexSize);
			ImGui::Text("boneCount: %i", boneCount);
			ImGui::Text("BoundBox min: %f, %f, %f", boundBox.min[0], boundBox.min[1], boundBox.min[2]);
			ImGui::Text("BoundBox max: %f, %f, %f", boundBox.max[0], boundBox.max[1], boundBox.max[2]);
			ImGui::Text("boneCount: %i", lodCount);
			ImGui::Text("materialCount: %i", materialCount);
			ImGui::Text("Materials:");
			for (u32 i = 0; i < materialCount; i++)
			{
				Material& mat = materials[i];
				if (ImGui::TreeNode(mat.name.c_str()))
				{
					ImGui::Text("shaderName: %s", mat.shaderName.c_str());
					ImGui::Text("sourcePath: %s", mat.sourcePath.c_str());
					ImGui::Text("attributeCount: %i", mat.attributeCount);
					for (u32 i = 0; i < mat.attributes.size(); i++)
					{
						ImGui::Text("attributeName: %s", mat.attributes[i].name.c_str());
						ImGui::Text("type: %i", mat.attributes[i].type);
						switch (mat.attributes[i].type)
						{
						case FLOAT:
						{
							ImGui::Text("value: %f", mat.attributes[i].valueFloat);
							break;
						}
						case VEC2:
						{
							ImGui::Text("value: %f %f", mat.attributes[i].valueVec2[0], mat.attributes[i].valueVec2[1]);
							break;
						}
						case VEC3:
						{
							ImGui::Text("value: %f %f %f", mat.attributes[i].valueVec3[0], mat.attributes[i].valueVec3[1], mat.attributes[i].valueVec3[2]);
							break;
						}
						case VEC4:
						{
							ImGui::Text("value: %f %f %f %f", mat.attributes[i].valueVec4[0], mat.attributes[i].valueVec4[1], mat.attributes[i].valueVec4[2], mat.attributes[i].valueVec4[3]);
							break;
						}
						case SAMPLER:
						{
							ImGui::Text("value: no data");
							break;
						}
						case TEXTURE:
						{
							ImGui::Text("value: %s", mat.attributes[i].valueTexture.c_str());
							break;
						}
						case BOOL:
						{
							ImGui::Text("value: %s", mat.attributes[i].valueBool ? "True" : "False");
							break;
						}
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::Text("MeshCount: %i", meshCount);
			for (u32 i = 0; i < meshCount; i++)
			{
				Mesh* mesh = &meshes[i];
				if (ImGui::TreeNode(std::format("Mesh {}", i).c_str()))
				{
					ImGui::Text("lod: %i", mesh->lod);
					ImGui::Text("vertexCount: %i", mesh->vertexCount);
					ImGui::Text("faceCount: %i", mesh->faceCount);
					ImGui::Text("vertexOffset: %i", mesh->vertexOffset);
					ImGui::Text("positionOnlyVertexOffset: %i", mesh->positionOnlyVertexOffset);
					ImGui::Text("faceOffset: %i", mesh->faceOffset);
					ImGui::Text("vertexAttributeCount: %i", mesh->vertexAttributeCount);
					ImGui::Text("boneCount: %i", mesh->boneCount);
					ImGui::TreePop();
				}
			}
			
		}
		else
		{
			ImGui::Text("unsupported version");
		}
	}
}