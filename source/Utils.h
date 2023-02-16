#pragma once
#include <fstream>

namespace dae
{
	namespace Utils
	{
		//These are copied from a fellow student
		inline Vector2 RandomGradient(int ix, int iy)
		{
			// No precomputed gradients mean this works for any number of grid coordinates
			const unsigned w = 8 * sizeof(unsigned);
			const unsigned s = w / 2; // rotation width
			unsigned a = ix, b = iy;
			a *= 3284157443; b ^= a << s | a >> w - s;
			b *= 1911520717; a ^= b << s | b >> w - s;
			a *= 2048419325;
			float random = static_cast<float>(a * (3.14159265 / ~(~0u >> 1))); // in [0, 2*Pi]
			Vector2 v;
			v.x = cos(random); v.y = sin(random);
			return v;
		}
		inline float Lerp(float x, float y, float a)
		{
			return x + a * (y - x);
		}
		inline float PerlinFunction(float x, float y, int octave, float zoom, float offset = 0.f)
		{
			x += offset;
			y += offset;

			x *= powf(2.0f, static_cast<float>(octave - 1));
			y *= powf(2.0f, static_cast<float>(octave - 1));

			x /= zoom;
			y /= zoom;

			const int gridX0{ static_cast<int>(x) };
			const int gridX1{ gridX0 + 1 };
			const int gridY0{ static_cast<int>(y) };
			const int gridY1{ gridY0 + 1 };

			const float gridPosX{ x - gridX0 };
			const float gridPosY{ y - gridY0 };

			const float dotGradient0{ Vector2::Dot(RandomGradient(gridX0, gridY0), Vector2{ gridPosX, gridPosY }) };
			const float dotGradient1{ Vector2::Dot(RandomGradient(gridX1, gridY0), Vector2{ gridPosX - 1.0f, gridPosY }) };
			const float dotGradient2{ Vector2::Dot(RandomGradient(gridX0, gridY1), Vector2{ gridPosX, gridPosY - 1.0f }) };
			const float dotGradient3{ Vector2::Dot(RandomGradient(gridX1, gridY1), Vector2{ gridPosX - 1.0f, gridPosY - 1.0f }) };

			const float xEase{ 3.0f * powf(gridPosX, 2.0f) - 2.0f * powf(gridPosX, 3.0f) };
			const float yEase{ 3.0f * powf(gridPosY, 2.0f) - 2.0f * powf(gridPosY, 3.0f) };

			return Lerp(Lerp(dotGradient0, dotGradient1, xEase), Lerp(dotGradient2, dotGradient3, xEase), yEase);
		}

		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, bool flipAxisAndWinding = true)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::vector<Vector3> positions{};
			std::vector<Vector3> normals{};
			std::vector<Vector2> UVs{};

			vertices.clear();
			indices.clear();

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;

					positions.emplace_back(x, y, z);
				}
				else if (sCommand == "vt")
				{
					// Vertex TexCoord
					float u, v;
					file >> u >> v;
					UVs.emplace_back(u, 1 - v);
				}
				else if (sCommand == "vn")
				{
					// Vertex Normal
					float x, y, z;
					file >> x >> y >> z;

					normals.emplace_back(x, y, z);
				}
				else if (sCommand == "f")
				{
					//if a face is read:
					//construct the 3 vertices, add them to the vertex array
					//add three indices to the index array
					//add the material index as attibute to the attribute array
					//
					// Faces or triangles
					Vertex vertex{};
					size_t iPosition, iTexCoord, iNormal;

					uint32_t tempIndices[3];
					for (size_t iFace = 0; iFace < 3; iFace++)
					{
						// OBJ format uses 1-based arrays
						file >> iPosition;
						vertex.position = positions[iPosition - 1];

						if ('/' == file.peek())//is next in buffer ==  '/' ?
						{
							file.ignore();//read and ignore one element ('/')

							if ('/' != file.peek())
							{
								// Optional texture coordinate
								file >> iTexCoord;
								vertex.uv = UVs[iTexCoord - 1];
							}

							if ('/' == file.peek())
							{
								file.ignore();

								// Optional vertex normal
								file >> iNormal;
								//vertex.normal = normals[iNormal - 1];
							}
						}

						vertices.push_back(vertex);
						tempIndices[iFace] = uint32_t(vertices.size()) - 1;
						//indices.push_back(uint32_t(vertices.size()) - 1);
					}

					indices.push_back(tempIndices[0]);
					if (flipAxisAndWinding) 
					{
						indices.push_back(tempIndices[2]);
						indices.push_back(tempIndices[1]);
					}
					else
					{
						indices.push_back(tempIndices[1]);
						indices.push_back(tempIndices[2]);
					}
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');
			}

			//Cheap Tangent Calculations
			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				uint32_t index0 = indices[i];
				uint32_t index1 = indices[size_t(i) + 1];
				uint32_t index2 = indices[size_t(i) + 2];

				const Vector3& p0 = vertices[index0].position;
				const Vector3& p1 = vertices[index1].position;
				const Vector3& p2 = vertices[index2].position;
				const Vector2& uv0 = vertices[index0].uv;
				const Vector2& uv1 = vertices[index1].uv;
				const Vector2& uv2 = vertices[index2].uv;

				const Vector3 edge0 = p1 - p0;
				const Vector3 edge1 = p2 - p0;
				const Vector2 diffX = Vector2(uv1.x - uv0.x, uv2.x - uv0.x);
				const Vector2 diffY = Vector2(uv1.y - uv0.y, uv2.y - uv0.y);
				float r = 1.f / Vector2::Cross(diffX, diffY);

				//Vector3 tangent = (edge0 * diffY.y - edge1 * diffY.x) * r;
				//vertices[index0].tangent += tangent;
				//vertices[index1].tangent += tangent;
				//vertices[index2].tangent += tangent;
			}

			//Create the Tangents (reject)
			for (auto& v : vertices)
			{
				//v.tangent = Vector3::Reject(v.tangent, v.normal).Normalized();

				if(flipAxisAndWinding)
				{
					v.position.z *= -1.f;
					//v.normal.z *= -1.f;
					//v.tangent.z *= -1.f;
				}

			}

			return true;
		}
#pragma warning(pop)
	}
}