#include "fontclass.h"
#include <fstream>
FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, const char* fontFilename, const char* textureFilename)
{
	bool result;


	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(const char* filename)
{
	using namespace std;
	ifstream fin;
	int i;
	char temp;
	
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}
	
	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (m_Font)
	{
		delete m_Font;
		m_Font = nullptr;
	}

	return;
}

bool FontClass::LoadTexture(ID3D11Device* device, const char* filename)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;
	
		// Draw each letter onto a quad.
		for (i = 0; i < numLetters; i++)
		{
			letter = ((int)sentence[i]) - 32;

			// If the letter is a space then just move over three pixels.
			if (letter == 0)
			{
				drawX = drawX + 3.0f;
			}
			else
			{
				// First triangle in quad.
				vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
				index++;

				vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
				vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
				index++;

				vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
				vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
				index++;

				// Second triangle in quad.
				vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
				vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
				index++;

				vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
				vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
				index++;

				vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
				vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
				index++;

				drawX = drawX + m_Font[letter].size + 1.0f;
			}
		}

	return;
}