//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dUtility.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides utility functions for simplifying common tasks.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////



#include "d3dUtility.h"
#include "cube.h"

extern int tiles[2][13][15];

map<int , Bomb>::iterator iter;
// vertex formats
//const DWORD  Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	struct Vertex
	{
		Vertex(){}
		Vertex(float x, float y, float z, 
			float nx, float ny, float nz,
			float u, float v)
		{
			_x  = x;  _y  = y;  _z  = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u  = u;  _v  = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v;

		static const DWORD FVF;
	};

	const DWORD  Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	enum Square{NONE, BOX, SAND, PLAYER};

	
/*Square g_square[13][15]=
{
	SAND,NONE,NONE,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,NONE,NONE,SAND,
	SAND,NONE,NONE,SAND,SAND,SAND,NONE,NONE,NONE,SAND,SAND,SAND,NONE,NONE,SAND,
	SAND,SAND,NONE,SAND,SAND,NONE,BOX,BOX,BOX,NONE,SAND,SAND,NONE,SAND,SAND,
	SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,
	SAND,SAND,SAND,NONE,BOX,SAND,SAND,SAND,SAND,SAND,BOX,NONE,SAND,SAND,SAND,
	SAND,SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,SAND,
	SAND,NONE,BOX,SAND,SAND,SAND,NONE,NONE,NONE,SAND,SAND,SAND,BOX,NONE,SAND,
	SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,
	SAND,NONE,BOX,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,SAND,BOX,NONE,SAND,
	SAND,NONE,BOX,NONE,SAND,SAND,SAND,SAND,SAND,SAND,SAND,NONE,BOX,NONE,SAND,
	SAND,SAND,NONE,BOX,NONE,SAND,NONE,BOX,NONE,SAND,NONE,BOX,NONE,SAND,SAND,
	SAND,NONE,SAND,NONE,BOX,BOX,BOX,NONE,BOX,BOX,BOX,NONE,SAND,NONE,SAND,
	SAND,SAND,SAND,SAND,NONE,NONE,NONE,SAND,NONE,NONE,NONE,SAND,SAND,SAND,SAND
};
*/
BOOL  InitD3D(
	HINSTANCE hInstance,
	int width, int height,
	BOOL windowed,
	HWND hwnd,
	D3DDEVTYPE deviceType,
	IDirect3DDevice9** device)
{


	//
	// Create the main application window.
	//
	DWORD		dwExStyle;
	DWORD		dwStyle;

	WNDCLASS wc;
	if (!windowed)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}
	

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;// CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;//(WNDPROC) WndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "My_Game";

	if( !RegisterClass(&wc) ) 
	{
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}
	
			//根据游戏大小计算窗口大小和位置
	int iWindowWidth = width + GetSystemMetrics(SM_CXFIXEDFRAME)*2,
		iWindowHeight = height + GetSystemMetrics(SM_CYFIXEDFRAME)*2 +
		GetSystemMetrics(SM_CYCAPTION);
	if(wc.lpszMenuName !=NULL)
		iWindowHeight +=GetSystemMetrics(SM_CYMENU);
	int iXWindowPos = (GetSystemMetrics(SM_CXSCREEN) - iWindowWidth)/2,
		iYWindowPos = (GetSystemMetrics(SM_CYSCREEN) - iWindowHeight)/2;


//	HWND hwnd = 0;
	hwnd = ::CreateWindowEx(dwExStyle,"My_Game", "My_Game", 
		dwStyle,
		iXWindowPos, iYWindowPos, width, height,
		0, //parent hwnd, 
		0, // menu //, 
		hInstance, 
		0 //extra//
		); 

	if( !hwnd )
	{
		::MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd); 

	//
	// Init D3D: 
	//

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.

	IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if( !d3d9 )
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
 
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
	    &d3dpp,             // present parameters
	    device);            // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			device);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object
	
	return true;
}

/*int  EnterMsgLoop( bool (*ptr_display)(float timeDelta) )
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime(); 

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
        {	
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			ptr_display(timeDelta);

			lastTime = currTime;
        }
    }
    return msg.wParam;
}
*/
D3DLIGHT9  InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9  InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_POINT;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9  InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_SPOT;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Direction = *direction;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta        = 0.5f;
	light.Phi          = 0.7f;

	return light;
}

D3DMATERIAL9  InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

 BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x =  INFINITY;
	_min.y =  INFINITY;
	_min.z =  INFINITY;

	_max.x = - INFINITY;
	_max.y = - INFINITY;
	_max.z = - INFINITY;
}

bool  BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}

 BoundingSphere::BoundingSphere()
{
	_radius = 0.0f;
}

bool  DrawBasicScene(IDirect3DDevice9* device, float scale)
{
	static IDirect3DVertexBuffer9*	floor  = 0;
	static IDirect3DTexture9*		tex    = 0;
	static ID3DXMesh*				pillar = 0;
	static Cube*					Box = 0;
	static IDirect3DTexture9*		Texboxtop = 0;
	static IDirect3DTexture9*		Texboxsides = 0;
	static IDirect3DTexture9*		Texsandtop = 0;
	static IDirect3DTexture9*		Texsandsides = 0;
	static IDirect3DTexture9*		Texbackground = 0;
	static IDirect3DTexture9*		Texside = 0;
	static IDirect3DTexture9*		Texlogo = 0;
	static ID3DXMesh*				tool = 0;
	static IDirect3DVertexBuffer9*	background  = 0;


	HRESULT hr = 0;



	if( device == 0 )
	{
		if( floor && tex && pillar && tool && background)  //需要更多release
		{
			// they already exist, destroy them
			 Release<IDirect3DVertexBuffer9*>(floor);
			 Release<IDirect3DTexture9*>(tex);
			 Release<ID3DXMesh*>(pillar);
			 Release<ID3DXMesh*>(tool);
		}
	}
	else if( !floor && !tex && !pillar && !tool && !background)
	{
		// they don't exist, create them
		device->CreateVertexBuffer(
			6 * sizeof( Vertex),
			0, 
			 Vertex::FVF,
			D3DPOOL_MANAGED,
			&floor,
			0);

		Vertex* v = 0;
		floor->Lock(0, 0, (void**)&v, 0);

		v[0] = Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 6.5f);
		v[1] = Vertex(0.0f, 0.0f,  26.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex( 30.0f, 0.0f,  26.0f, 0.0f, 1.0f, 0.0f, 7.5f, 0.0f);

		v[3] = Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 6.5f);
		v[4] = Vertex( 30.0f, 0.0f,  26.0f, 0.0f, 1.0f, 0.0f, 7.5f, 0.0f);
		v[5] = Vertex( 30.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 7.5f, 6.5f);

		floor->Unlock();



		device->CreateVertexBuffer(
			6 * sizeof( Vertex),
			0, 
			 Vertex::FVF,
			D3DPOOL_MANAGED,
			&background,
			0);

		Vertex* vv = 0;
		background->Lock(0, 0, (void**)&vv, 0);

		vv[0] = Vertex(-100.0f, -100.0f, -22.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		vv[1] = Vertex( 100.0f, 100.0f,  74.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		vv[2] = Vertex(100.0f, -100.0f,  -22.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		vv[3] = Vertex(100.0f, 100.0f, 74.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		vv[4] = Vertex( -100.0f, -100.0f,  -22.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		vv[5] = Vertex( -100.0f, 100.0f, 74.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);

/*		vv[0] = Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 6.5f);
		vv[1] = Vertex(0.0f, 0.0f,  26.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		vv[2] = Vertex( 30.0f, 0.0f,  26.0f, 0.0f, 1.0f, 0.0f, 7.5f, 0.0f);

		vv[3] = Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 6.5f);
		vv[4] = Vertex( 30.0f, 0.0f,  26.0f, 0.0f, 1.0f, 0.0f, 7.5f, 0.0f);
		vv[5] = Vertex( 30.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 7.5f, 6.5f);
*/
		background->Unlock();

		D3DXCreateCylinder(device, 1.0f, 0.8f, 2.0f, 20, 20, &pillar, 0);
		D3DXCreateSphere(device, 1.0f, 20, 20, &tool, 0);

		Box = new Cube(device);
	//
	// Create texture.
	//



		D3DXCreateTextureFromFile(
			device,
			"desert.bmp",
			&tex);

		D3DXCreateTextureFromFile(
			device,
			"boxtop.bmp",
			&Texboxtop);

		D3DXCreateTextureFromFile(
			device,
			"boxsides.bmp",
			&Texboxsides);

		D3DXCreateTextureFromFile(
			device,
			"sandtop.bmp",
			&Texsandtop);

		D3DXCreateTextureFromFile(
			device,
			"sandsides.bmp",
			&Texsandsides);
		D3DXCreateTextureFromFile(
			device,
			"background.bmp",
			&Texbackground);
		D3DXCreateTextureFromFile(
			device,
			"side.bmp",
			&Texside);
		D3DXCreateTextureFromFile(
			device,
			"logo.bmp",
			&Texlogo);


		
			// 
	// Set Texture Filter States.
	//

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	}
	else
	{
		//
		// Pre-Render Setup
		//
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

//		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light =  InitDirectionalLight(&dir, &col);

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);

		//
		// Render
		//

		D3DXMATRIX T, R, P, S;

		D3DXMatrixScaling(&S, scale, scale, scale);

		// used to rotate cylinders to be parallel with world's y-axis
		D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);

		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
		device->SetTransform(D3DTS_WORLD, &T);
		device->SetMaterial(& WHITE_MTRL);
		device->SetTexture(0, tex);
		device->SetStreamSource(0, floor, 0, sizeof(Vertex));
		device->SetFVF(Vertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		
		// draw background
		D3DXMatrixIdentity(&T);
		T = T * S;
		device->SetTransform(D3DTS_WORLD, &T);
		device->SetMaterial(& WHITE_MTRL);
		device->SetTexture(0, Texbackground);
		device->SetStreamSource(0, background, 0, sizeof(Vertex));
		device->SetFVF(Vertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		// draw pillars
//		device->SetMaterial(& BLUE_MTRL);
		device->SetMaterial(& BLACK_MTRL);
		device->SetTexture(0, 0);
/*		for(int i = 0; i < 5; i++)
		{
			D3DXMatrixTranslation(&T, -5.0f, 0.0f, -15.0f + (i * 7.5f));
			P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);
			pillar->DrawSubset(0);

			D3DXMatrixTranslation(&T, 5.0f, 0.0f, -15.0f + (i * 7.5f));
			P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);
			pillar->DrawSubset(0);
		}
*/		
		D3DXMatrixTranslation(&T, 3.0f, 1.0f, 3.0f);
		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		pillar->DrawSubset(0);
	//	tool->DrawSubset(0);

		D3DXMatrixTranslation(&T, 3.0f, 1.0f, 23.0f);
		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		pillar->DrawSubset(0);

		D3DXMatrixTranslation(&T, 27.0f, 1.0f, 23.0f);
		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		pillar->DrawSubset(0);

		D3DXMatrixTranslation(&T, 27.0f, 1.0f, 3.0f);
		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		pillar->DrawSubset(0);

	/*	D3DXMatrixTranslation(&T, 15.0f, 1.0f, 13.0f);
		P =  T * S;
		device->SetTransform(D3DTS_WORLD, &P);*/
		device->SetMaterial(& WHITE_MTRL);

		D3DXMatrixTranslation(&T, 13, 3.0f, 13);
		P =   T * S; 
		device->SetTransform(D3DTS_WORLD, &P);
		Box->draw(0, 0, Texlogo, Texside);

		D3DXMatrixTranslation(&T, 13, 1.0f, 13);
		P =   T * S; 
		device->SetTransform(D3DTS_WORLD, &P);
		Box->draw(0, 0, Texlogo, Texside);

		D3DXMatrixTranslation(&T, 17, 3.0f, 13);
		P =  T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		Box->draw(0, 0, Texlogo, Texside);

		D3DXMatrixTranslation(&T, 17, 1.0f, 13);
		P =  T * S;
		device->SetTransform(D3DTS_WORLD, &P);
		Box->draw(0, 0, Texlogo, Texside);

	
		for(int i = 0 ; i < 13 ; i++)
			for(int j  = 0 ; j < 15 ; j++)
			{
		//		D3DXMatrixTranslation(&T, i*2+1, 1.0f, j*2+1);
				D3DXMatrixTranslation(&T, j*2+1, 1.0f, i*2+1);

				P =  T * S;
				device->SetTransform(D3DTS_WORLD, &P);

				
				if(tiles[MyMap::LAYER_BUILDING ][i][j] == MyMap::BOX)
				{
					device->SetMaterial(& WHITE_MTRL);
					Box->draw(0, 0, Texboxtop,Texboxsides);
				}
				if(tiles[MyMap::LAYER_BUILDING ][i][j] == MyMap::SAND)
				{
					device->SetMaterial(& WHITE_MTRL);
					Box->draw(0, 0, Texsandtop,Texsandsides);
				}
				if(tiles[MyMap::LAYER_BUILDING ][i][j] == MyMap::TOOL_BOMB )
				{
					device->SetMaterial(& BLUE_MTRL);
					device->SetTexture(0, 0);
					tool->DrawSubset(0);
				}
				if(tiles[MyMap::LAYER_BUILDING ][i][j] == MyMap::TOOL_POWER  )
				{
					device->SetMaterial(& YELLOW_MTRL);
					device->SetTexture(0, 0);
					tool->DrawSubset(0);
				}
				if(tiles[MyMap::LAYER_BUILDING ][i][j] == MyMap::TOOL_POWER_MAX  )
				{
					device->SetMaterial(& RED_MTRL);
					device->SetTexture(0, 0);
					tool->DrawSubset(0);
				}
				if(tiles[MyMap::LAYER_BUILDING ][i][j] == MyMap::TOOL_SPEED  )
				{
					device->SetMaterial(& GREEN_MTRL);
					device->SetTexture(0, 0);
					tool->DrawSubset(0);
				}


			}

	}
	return true;
}

float  GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

void  GetRandomVector(
	  D3DXVECTOR3* out,
	  D3DXVECTOR3* min,
	  D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

DWORD  FtoDw(float f)
{
	return *((DWORD*)&f);
}


bool  DrawPlayer(IDirect3DDevice9* device, float scale, player player1)
{


	if(player1.m_PlayState == DIE) 
		return false;
	static ID3DXMesh*                      Mesh = 0;
	static std::vector<D3DMATERIAL9>       Mtrls(0);
	static std::vector<IDirect3DTexture9*> Textures(0);
	static ID3DXMesh*						Sphere = 0;
	static D3DMATERIAL9						blue;

	HRESULT hr = 0;

	if( device == 0 )
	{
		if( Mesh   )  //需要更多release
		{
			// they already exist, destroy them
	//		 Release<ID3DMATERIAL9>(Mtrls);
	//		 Release<IDirect3DTexture9*>(Textures);
			 Release<ID3DXMesh*>(Mesh);
		}
	}
	else if( !Mesh )//&& !Mtrls && !Textures )
	{
		// they don't exist, create them
		ID3DXBuffer* adjBuffer  = 0;
		ID3DXBuffer* mtrlBuffer = 0;
		DWORD        numMtrls   = 0;

	hr = D3DXLoadMeshFromX(  
		"bigship1.x",
	//	"Tiny.x",
		D3DXMESH_MANAGED,
		device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					device,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}
	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//
	// Optimize the mesh.
	//

	hr = Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}



	}
	else
	{
	//
	// Set texture filters.
	//
		if(!Sphere)
			D3DXCreateSphere(device, 150.0f, 20, 20, &Sphere, 0);

			blue = BLUE_MTRL;
			blue.Diffuse.a = 0.40f;


		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

/*		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light =  InitDirectionalLight(&dir, &col);

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
*/
		//
		// Render
		//

		D3DXMATRIX T, R, P, S, Y, Z, YY;

		D3DXMatrixScaling(&S, scale, scale, scale);


		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
//		device->SetTransform(D3DTS_WORLD, &T);
//		device->SetMaterial(& WHITE_MTRL);
//		device->SetTexture(0, tex);
//		device->SetStreamSource(0, floor, 0, sizeof(Vertex));
//		device->SetFVF(Vertex::FVF);
//		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		
//		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		device->BeginScene();


//		D3DXMatrixTranslation(&T, 500.0f, 1.0f, 500.0f); //偏移量！
//		float x = 2.5;
//		D3DXMatrixTranslation(&T, 100*(2*x-1), 1.0f, 500.0f); //偏移量！
		D3DXMatrixTranslation(&T, 100 * (2 * player1.m_fX + 1), 10.0f, 100 * ( 2 * player1.m_fY + 1)); //偏移量！
		D3DXMatrixRotationX(&R, D3DX_PI * 0.5f);
		D3DXMatrixRotationY(&Y, D3DX_PI * 1.0f);
//		D3DXMatrixRotationZ(&Z, D3DX_PI * 1.0f);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 

		D3DXMatrixRotationZ(&Z, D3DX_PI * 0.5*player1.m_Direction);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 


//		D3DXMatrixTranslation(&T, 27.0f, 1.0f, 23.0f);

		P =  Z * Y * R * T * S;
//		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);

		if(player1.m_PlayState == ALIVE)
		{

			for(int i = 0; i < Mtrls.size(); i++)
			{
				device->SetMaterial( &Mtrls[i] );
				device->SetTexture(0, Textures[i]);
				Mesh->DrawSubset(i);
			}	
		}

		if(player1.m_PlayState == DYING)
		{
			device->SetMaterial(&blue);
			device->SetTexture(0, 0); // disable texture

			device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

//			D3DXMatrixTranslation(&YY, 0, 4, 0);
			D3DXMatrixTranslation(&YY, 0, 1, 0);
			P = P * YY;
			device->SetTransform(D3DTS_WORLD, &P);

			Sphere->DrawSubset(0);
		}

		device->EndScene();
		device->Present(0, 0, 0, 0);
		

	}
	return true;
}

//----------------------------------
//DrawPlayer2
//-----------------------------------

bool  DrawPlayer2(IDirect3DDevice9* device, float scale, player player1)
{


	if(player1.m_PlayState == DIE) 
		return false;
	static ID3DXMesh*                      Mesh = 0;
	static std::vector<D3DMATERIAL9>       Mtrls(0);
	static std::vector<IDirect3DTexture9*> Textures(0);
	static ID3DXMesh*						Sphere = 0;
	static D3DMATERIAL9						blue;

	HRESULT hr = 0;

	if( device == 0 )
	{
		if( Mesh   )  //需要更多release
		{
			// they already exist, destroy them
	//		 Release<ID3DMATERIAL9>(Mtrls);
	//		 Release<IDirect3DTexture9*>(Textures);
			 Release<ID3DXMesh*>(Mesh);
		}
	}
	else if( !Mesh )//&& !Mtrls && !Textures )
	{
		// they don't exist, create them
		ID3DXBuffer* adjBuffer  = 0;
		ID3DXBuffer* mtrlBuffer = 0;
		DWORD        numMtrls   = 0;

	hr = D3DXLoadMeshFromX(  
	//	"bigship1.x",
		"Tiny.x",
		D3DXMESH_MANAGED,
		device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					device,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}
	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//
	// Optimize the mesh.
	//

	hr = Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}



	}
	else
	{
	//
	// Set texture filters.
	//
		if(!Sphere)
			D3DXCreateSphere(device, 150.0f, 20, 20, &Sphere, 0);

			blue = GREEN_MTRL;
			blue.Diffuse.a = 0.40f;


		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

/*		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light =  InitDirectionalLight(&dir, &col);

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
*/
		//
		// Render
		//

		D3DXMATRIX T, R, P, S, Y, Z, YY;

		D3DXMatrixScaling(&S, scale, scale, scale);


		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
		device->BeginScene();


//		D3DXMatrixTranslation(&T, 500.0f, 1.0f, 500.0f); //偏移量！
//		float x = 2.5;
//		D3DXMatrixTranslation(&T, 100*(2*x-1), 1.0f, 500.0f); //偏移量！
		D3DXMatrixTranslation(&T, 100 * (2 * player1.m_fX + 1), 300.0f, 100 * ( 2 * player1.m_fY + 1)); //偏移量！
		D3DXMatrixRotationX(&R, D3DX_PI * 0.5f);
		D3DXMatrixRotationY(&Y, D3DX_PI * 1.0f);
//		D3DXMatrixRotationZ(&Z, D3DX_PI * 1.0f);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 

		D3DXMatrixRotationZ(&Z, D3DX_PI * 0.5*player1.m_Direction);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 


//		D3DXMatrixTranslation(&T, 27.0f, 1.0f, 23.0f);

		P =  Z * Y * R * T * S;
//		P = R * T * S;
		device->SetTransform(D3DTS_WORLD, &P);

		if(player1.m_PlayState == ALIVE)
		{

			for(int i = 0; i < Mtrls.size(); i++)
			{
				device->SetMaterial( &Mtrls[i] );
				device->SetTexture(0, Textures[i]);
				Mesh->DrawSubset(i);
			}	
		}

		if(player1.m_PlayState == DYING)
		{
			device->SetMaterial(&blue);
			device->SetTexture(0, 0); // disable texture

			device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

//			D3DXMatrixTranslation(&YY, 0, 4, 0);
			D3DXMatrixTranslation(&YY, 0, 1, 0);
			P = P * YY;
			device->SetTransform(D3DTS_WORLD, &P);

			Sphere->DrawSubset(0);
		}

		device->EndScene();
		device->Present(0, 0, 0, 0);
		

	}
	return true;
}

// DrawPaopao

bool  DrawPaopao(IDirect3DDevice9* device, float scale,  map<int,Bomb> & ma)
{


	static ID3DXMesh*                      Mesh = 0;
	static std::vector<D3DMATERIAL9>       Mtrls(0);
	static std::vector<IDirect3DTexture9*> Textures(0);

	HRESULT hr = 0;

	if( device == 0 )
	{
		if( Mesh   )  //需要更多release
		{
			// they already exist, destroy them
	//		 Release<ID3DMATERIAL9>(Mtrls);
	//		 Release<IDirect3DTexture9*>(Textures);
			 Release<ID3DXMesh*>(Mesh);
		}
	}
	else if( !Mesh )//&& !Mtrls && !Textures )
	{
		// they don't exist, create them
		ID3DXBuffer* adjBuffer  = 0;
		ID3DXBuffer* mtrlBuffer = 0;
		DWORD        numMtrls   = 0;

	hr = D3DXLoadMeshFromX(  
		"bigship12.x",
		D3DXMESH_MANAGED,
		device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					device,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}
	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//
	// Optimize the mesh.
	//

	hr = Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}



	}
	else
	{
	//
	// Set texture filters.
	//

		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

/*		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light =  InitDirectionalLight(&dir, &col);

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
*/
		//
		// Render
		//

		D3DXMATRIX T, R, P, S, Y, Z;

		D3DXMatrixScaling(&S, scale, scale, scale);


		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
//		device->SetTransform(D3DTS_WORLD, &T);
//		device->SetMaterial(& WHITE_MTRL);
//		device->SetTexture(0, tex);
//		device->SetStreamSource(0, floor, 0, sizeof(Vertex));
//		device->SetFVF(Vertex::FVF);
//		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		
//		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		device->BeginScene();
//		D3DXMatrixTranslation(&T, 500.0f, 1.0f, 500.0f); //偏移量！
//		float x = 2.5;
//		D3DXMatrixTranslation(&T, 100*(2*x-1), 1.0f, 500.0f); //偏移量！
		D3DXMatrixRotationX(&R, D3DX_PI * 1.5f);
		D3DXMatrixRotationY(&Y, D3DX_PI * 1.5f);
		D3DXMatrixRotationZ(&Z, D3DX_PI * 1.0f);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 
//		map<int , Bomb>::iterator iter;
//		D3DXMatrixRotationZ(&Z, D3DX_PI * 0.5*player1.m_Direction);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 
		for (iter = ma.begin(); iter != ma.end(); iter++)

		{
			D3DXMatrixIdentity(&T);
			T = T * S;
			D3DXMatrixTranslation(&T,  (2 * (iter->second.m_iCol) + 1) /scale, 1.0f,  ( 2 * (iter->second.m_iRow) + 1) / scale); //偏移量！
			P =  Z * Y * R * T * S;
	//		P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &P);

			for(int i = 0; i < Mtrls.size(); i++)
			{
				device->SetMaterial( &Mtrls[i] );
				device->SetTexture(0, Textures[i]);
				Mesh->DrawSubset(i);
			}	
		}
		device->EndScene();
		device->Present(0, 0, 0, 0);
		

	}
	return true;
}

//---------------------------------------------------------
//DrawSky
//---------------------------------------------------------

bool  DrawSky(IDirect3DDevice9* device, float scale)
{


	static ID3DXMesh*                      Mesh = 0;
	static std::vector<D3DMATERIAL9>       Mtrls(0);
	static std::vector<IDirect3DTexture9*> Textures(0);

	HRESULT hr = 0;

	if( device == 0 )
	{
		if( Mesh   )  //需要更多release
		{
			// they already exist, destroy them
	//		 Release<ID3DMATERIAL9>(Mtrls);
	//		 Release<IDirect3DTexture9*>(Textures);
			 Release<ID3DXMesh*>(Mesh);
		}
	}
	else if( !Mesh )//&& !Mtrls && !Textures )
	{
		// they don't exist, create them
		ID3DXBuffer* adjBuffer  = 0;
		ID3DXBuffer* mtrlBuffer = 0;
		DWORD        numMtrls   = 0;

	hr = D3DXLoadMeshFromX(  
		"skybox2.x",
		D3DXMESH_MANAGED,
		device,
		&adjBuffer,
		&mtrlBuffer,
		0,
		&numMtrls,
		&Mesh);

	if(FAILED(hr))
	{
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}

	//
	// Extract the materials, and load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

		for(int i = 0; i < numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			Mtrls.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;
				D3DXCreateTextureFromFile(
					device,
					mtrls[i].pTextureFilename,
					&tex);

				// save the loaded texture
				Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				Textures.push_back( 0 );
			}
		}
	}
	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//
	// Optimize the mesh.
	//

	hr = Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}



	}
	else
	{
	//
	// Set texture filters.
	//

		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

/*		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light =  InitDirectionalLight(&dir, &col);

		device->SetLight(0, &light);
		device->LightEnable(0, true);
		device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		device->SetRenderState(D3DRS_SPECULARENABLE, true);
*/
		//
		// Render
		//

		D3DXMATRIX T, R, P, S, Y, Z;

		D3DXMatrixScaling(&S, scale, scale, scale);


		// draw floor
		D3DXMatrixIdentity(&T);
		T = T * S;
//		device->SetTransform(D3DTS_WORLD, &T);
//		device->SetMaterial(& WHITE_MTRL);
//		device->SetTexture(0, tex);
//		device->SetStreamSource(0, floor, 0, sizeof(Vertex));
//		device->SetFVF(Vertex::FVF);
//		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		
//		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		device->BeginScene();
//		D3DXMatrixTranslation(&T, 500.0f, 1.0f, 500.0f); //偏移量！
//		float x = 2.5;
//		D3DXMatrixTranslation(&T, 100*(2*x-1), 1.0f, 500.0f); //偏移量！
		D3DXMatrixRotationX(&R, D3DX_PI * 1.5f);
		D3DXMatrixRotationY(&Y, D3DX_PI * 1.5f);
		D3DXMatrixRotationZ(&Z, D3DX_PI * 1.0f);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 
//		map<int , Bomb>::iterator iter;
//		D3DXMatrixRotationZ(&Z, D3DX_PI * 0.5*player1.m_Direction);    //控制朝向！ 0.0下，0.5左，1.0上，1.5右 
			D3DXMatrixIdentity(&T);
			T = T * S;
	//		D3DXMatrixTranslation(&T,  (2 * (iter->second.m_iCol) + 1) /scale, 1.0f,  ( 2 * (iter->second.m_iRow) + 1) / scale); //偏移量！
			P =  Z * Y * R * T * S;
	//		P = R * T * S;
			device->SetTransform(D3DTS_WORLD, &S);

			for(int i = 0; i < Mtrls.size(); i++)
			{
				device->SetMaterial( &Mtrls[i] );
				device->SetTexture(0, Textures[i]);
				Mesh->DrawSubset(i);
			}	
		
		device->EndScene();
		device->Present(0, 0, 0, 0);
		

	}
	return true;
}