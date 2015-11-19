#include "Shader.h"
#include <d3dcompiler.h>

Shader::Shader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName)
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;

	m_initialized = Initialize(device, hwnd, shaderFileName);
}

Shader::~Shader(void)
{
	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	m_name.clear();
}

bool Shader::Initialize(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName)
{
	wstring shaderFilePath(shaderFileName);
	m_name = string(shaderFilePath.begin(), shaderFilePath.end());

	int pos = m_name.find_last_of("/");
	if(pos >= 0)
	{
		m_name = m_name.substr(pos + 1, m_name.length());
	}

	//string vsFileName = shaderFileName;
	//vsFileName += ".vs";

	//string psFileName = shaderFilePath;
	//psFileName += ".ps";

	//initialize the vertex and pixel shaders
	return InitializeShader(device, hwnd, shaderFileName);
}

void Shader::Begin(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//set the vertex input layout
	deviceContext->IASetInputLayout(m_layout);

	//Set the vertex and pixel shaders that will used to render
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//render
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void Shader::End(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(NULL);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
}

bool Shader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage = 0;
	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* pixelShaderBuffer = 0;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	//compile vertex shader
	result = D3DCompileFromFile(shaderFileName, NULL, NULL, "VSMain", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		//if we have an error message
		if(errorMessage)
		{
			OutputShadeErrorMessage(errorMessage, hwnd, shaderFileName);
		}
		else
		{
			MessageBox(hwnd, (LPCSTR)shaderFileName, "Error in VS Shader File", MB_OK);
		}

		return false;
	}

	//compile pixel shader
	result = D3DCompileFromFile(shaderFileName, NULL, NULL, "PSMain", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		//if we have an error message
		if(errorMessage)
		{
			OutputShadeErrorMessage(errorMessage, hwnd, shaderFileName);
		}
		else
		{
			MessageBox(hwnd, (LPCSTR)shaderFileName, "Error in PS Shader File", MB_OK);
		}
	}

	//create vertex shader buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	//create pixel shader buffer
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	//setup the layout of the data that goes into the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout)/sizeof(polygonLayout[0]);

	//create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
										vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	//release shader buffers
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//setup the matrix buffer description
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//create the constant buffer pointer
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void Shader::OutputShadeErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
{
	char* compileErrors = (char*)errorMessage->GetBufferPointer();
	unsigned long bufferSize = errorMessage->GetBufferSize();

	ofstream fout;
	//open file to write
	fout.open("shader-error-txt");

	for(unsigned int i = 0; i < bufferSize; ++i)
	{
		fout << compileErrors[i];
	}

	//close file
	fout.close();

	//release
	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, "Error compiling shader. Check shader-error.txt for message", (LPCSTR)shaderFileName, MB_OK);

	return;
}

bool Shader::SetShaderParamters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture)
{
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

bool Shader::SetShaderParamters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	/*D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);*/

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->worldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix));
	dataPtr->viewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&viewMatrix));
	dataPtr->projectionMatrix = XMMatrixTranspose(XMLoadFloat4x4(&projectionMatrix));

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

string Shader::GetName()
{
	return m_name;
}

bool Shader::IsInitialized()
{
	return m_initialized;
}
