#include "previews/ImagePreview.h"
#include "gli/load_dds.hpp"
#include "gli/gl.hpp"
#include "imgui.h"

namespace AWExtractor::UI
{
	GLuint create_dds(std::vector<u8> data)
	{
		gli::texture Texture = gli::load_dds((const char*)data.data(), data.size());
		if (Texture.empty())
			return 0;

		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
		GLenum Target = GL.translate(Texture.target());

		GLuint TextureName = 0;
		glGenTextures(1, &TextureName);
		glBindTexture(Target, TextureName);
		glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
		glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
		glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
		glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
		glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);

		glm::tvec3<GLsizei> const Extent(Texture.extent());
		GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

		switch (Texture.target())
		{
		case gli::TARGET_1D:
			glTexStorage1D(
				Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x);
			break;
		case gli::TARGET_1D_ARRAY:
		case gli::TARGET_2D:
		case gli::TARGET_CUBE:
			glTexStorage2D(
				Target, static_cast<GLint>(Texture.levels()), Format.Internal,
				Extent.x, Texture.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
			break;
		case gli::TARGET_2D_ARRAY:
		case gli::TARGET_3D:
		case gli::TARGET_CUBE_ARRAY:
			glTexStorage3D(
				Target, static_cast<GLint>(Texture.levels()), Format.Internal,
				Extent.x, Extent.y,
				Texture.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
			break;
		default:
			assert(0);
			break;
		}

		for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
			for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
				for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
				{
					GLsizei const LayerGL = static_cast<GLsizei>(Layer);
					glm::tvec3<GLsizei> Extent(Texture.extent(Level));
					Target = gli::is_target_cube(Texture.target())
						? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
						: Target;

					switch (Texture.target())
					{
					case gli::TARGET_1D:
						if (gli::is_compressed(Texture.format()))
							glCompressedTexSubImage1D(
								Target, static_cast<GLint>(Level), 0, Extent.x,
								Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
								Texture.data(Layer, Face, Level));
						else
							glTexSubImage1D(
								Target, static_cast<GLint>(Level), 0, Extent.x,
								Format.External, Format.Type,
								Texture.data(Layer, Face, Level));
						break;
					case gli::TARGET_1D_ARRAY:
					case gli::TARGET_2D:
					case gli::TARGET_CUBE:
						if (gli::is_compressed(Texture.format()))
							glCompressedTexSubImage2D(
								Target, static_cast<GLint>(Level),
								0, 0,
								Extent.x,
								Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
								Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
								Texture.data(Layer, Face, Level));
						else
							glTexSubImage2D(
								Target, static_cast<GLint>(Level),
								0, 0,
								Extent.x,
								Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
								Format.External, Format.Type,
								Texture.data(Layer, Face, Level));
						break;
					case gli::TARGET_2D_ARRAY:
					case gli::TARGET_3D:
					case gli::TARGET_CUBE_ARRAY:
						if (gli::is_compressed(Texture.format()))
							glCompressedTexSubImage3D(
								Target, static_cast<GLint>(Level),
								0, 0, 0,
								Extent.x, Extent.y,
								Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
								Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
								Texture.data(Layer, Face, Level));
						else
							glTexSubImage3D(
								Target, static_cast<GLint>(Level),
								0, 0, 0,
								Extent.x, Extent.y,
								Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
								Format.External, Format.Type,
								Texture.data(Layer, Face, Level));
						break;
					default: assert(0); break;
					}
				}
		return TextureName;
	}


	ImagePreview::ImagePreview(string fileName, std::shared_ptr<ExtractorCore::EndianReader> er) : FilePreview(fileName, er)
	{
		std::vector<u8> data = er->ReadBytes(er->GetSize());
		try
		{
			texID = create_dds(data);

			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		}
		catch (...)
		{
			texID = 0;
		}
	}

	void ImagePreview::Draw()
	{
		if (texID != 0)
		{
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();

			float widthRatio = windowWidth / width;
			float heightRatio = windowHeight / height;

			float outWidth = 0;
			float outHeight = 0;

			if (widthRatio <= heightRatio)
			{
				outWidth = width * widthRatio;
				outHeight = height * widthRatio;
			}
			else
			{
				outWidth = width * heightRatio;
				outHeight = height * heightRatio;
			}

			ImGui::Image((void*)texID, ImVec2(outWidth, outHeight));
		}
		else
		{
			ImGui::Text("Failed to load image");
		}
	}
	ImagePreview::~ImagePreview()
	{
		glDeleteTextures(1, &texID);
	}
}