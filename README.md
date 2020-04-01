# Cog Graphics Framework

An old salvaged toy engine written in C.

Graphics framework written in C99/gcc aimed towards ease of use for gaming, easy environment setup for graphical demos and proof of concepts

## Included modules (some are, some I never wrote):

### Cog Data: (libcogdata)
	List
	Array
	Hash
	String
	Type Helpers

### Cog Math: (libcogmath)
	Vector2
	Vector3
	Vector4
	Quat3
	Quat4
	Matrix2
	Matrix3
	Matrix4
	CatmullRom2
	CatmullRom3
	Bezier2
	Bezier3
	Rectangle
	Circle
	Box
	Sphere

### Cog Platform: (libcogplatform)
	Mutex
	Thread
	Socket
	Connection
	Mouse
	Keyboard
	Controller
	Window

### Cog Utils: (libcogutils)
	Logger

### Cog Graphics (libcoggraphics)
	RenderSystem
		Color
		Camera
		Light
		FrameBuffer
		RenderTarget
		RenderLayer
		RendererInterface
		RendererForwardBuiltin
		RendererDeferredBuiltin
		RendererRayTracerBuiltin
	ResourceManager
		Mesh
		Skeleton
		Sprite
		Texture
		Image
		Shader
		Material
		Font
		Localization
	SceneManager
		SceneInterface
	GuiSystem
		GuiElementInterface
		GuiButton
		GuiLabel
		GuiTextfield
		GuiTextbox
		GuiWindow
	QuadTree
	OctTree
	BSPTree

### Cog Audio: (libcogaudio)
	AudioManager
		AudioSource
	Mixer
	Player
