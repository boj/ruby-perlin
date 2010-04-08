require '../../perlin'
require 'rubygems'
require 'fox16'

# Base framework borrowed from: http://www.fxruby.org/examples/gltest.rb

include Fox

class GLTestWindow < FXMainWindow

  # How often our timer will fire (in milliseconds)
  TIMER_INTERVAL = 100
  
  def drawTerrain(width, length)
    p = Perlin.new(rand(1000), 0.5, 1)
    for x in -width..width
      for y in -length..length
        z = p.run(x, y)
        GL.Begin(GL::TRIANGLE_STRIP)
          GL.Normal(0.0, 0.0, 0.0)
          GL.Vertex(x.to_f, y.to_f, z.to_f)
          GL.Vertex(x.to_f, y.to_f + 1.0, z.to_f)
          GL.Vertex(x.to_f + 1.0, y.to_f, z.to_f)
          GL.Vertex(x.to_f + 1.0, y.to_f + 1.0, z.to_f)
        GL.End()
      end
    end
  end

  # Draw the GL scene
  def drawScene
    lightPosition = [15.0, 10.0, 5.0, 1.0]
    lightAmbient  = [ 0.5,  0.5, 0.5, 1.0]
    lightDiffuse  = [ 0.9,  0.9, 0.9, 1.0]
    blueMaterial  = [ 0.0,  0.0, 1.0, 1.0]
  
    width = @glcanvas.width.to_f
    height = @glcanvas.height.to_f
    aspect = width / height
  
    # Make context current
    @glcanvas.makeCurrent()
    
    GL.Viewport(0, 0, @glcanvas.width, @glcanvas.height)
  
    GL.ClearColor(0.0, 0.0, 0.0, 0.0)
    GL.Clear(GL::COLOR_BUFFER_BIT|GL::DEPTH_BUFFER_BIT)
    GL.Enable(GL::DEPTH_TEST)
    
    GL.Disable(GL::DITHER)
  
    GL.MatrixMode(GL::PROJECTION)
    GL.LoadIdentity()
    GLU.Perspective(90.0, aspect, 1.0, 100.0)
  
    GL.MatrixMode(GL::MODELVIEW)
    GL.LoadIdentity()
    GLU.LookAt(0.0, 15.0, 10.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0)
  
    GL.ShadeModel(GL::SMOOTH)
    GL.Light(GL::LIGHT0, GL::POSITION, lightPosition)
    GL.Light(GL::LIGHT0, GL::AMBIENT, lightAmbient)
    GL.Light(GL::LIGHT0, GL::DIFFUSE, lightDiffuse)
    GL.Enable(GL::LIGHT0)
    GL.Enable(GL::LIGHTING)
  
    GL.Material(GL::FRONT, GL::AMBIENT, blueMaterial)
    GL.Material(GL::FRONT, GL::DIFFUSE, blueMaterial)
  
    GL.PushMatrix()
    GL.Rotated(0.0, 0.0, 1.0, 0.0)
    drawTerrain(10, 10)
    GL.PopMatrix()
  
    # Swap if it is double-buffered
    if @glvisual.isDoubleBuffer
      @glcanvas.swapBuffers
    end
    
    # Make context non-current
    @glcanvas.makeNonCurrent
  end

  def initialize(app)
    # Invoke the base class initializer
    super(app, "Perlin Visualizer", :opts => DECOR_ALL, :width => 800, :height => 600)

    # Construct the main window elements
    frame = FXHorizontalFrame.new(self, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y)
    frame.padLeft, frame.padRight = 0, 0
    frame.padTop, frame.padBottom = 0, 0

    # Left pane to contain the glcanvas
    glcanvasFrame = FXVerticalFrame.new(frame, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT)
  
    # Drawing glcanvas
    glpanel = FXVerticalFrame.new(
      glcanvasFrame, (FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT)
    )
    glpanel.padLeft, glpanel.padRight = 0, 0
    glpanel.padTop, glpanel.padBottom = 0, 0
      
    # A visual to draw OpenGL
    @glvisual = FXGLVisual.new(getApp(), VISUAL_DOUBLEBUFFER)
  
    # Drawing glcanvas
    @glcanvas = FXGLCanvas.new(glpanel, @glvisual, :opts => LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT)
    @glcanvas.connect(SEL_PAINT) { drawScene }
    @glcanvas.connect(SEL_CONFIGURE) do
      if @glcanvas.makeCurrent
        GL.Viewport(0, 0, @glcanvas.width, @glcanvas.height)
        @glcanvas.makeNonCurrent
      end
    end
    
    # Right pane for the buttons
    buttonFrame = FXVerticalFrame.new(frame, LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT)
    buttonFrame.padLeft, buttonFrame.padRight = 10, 10
    buttonFrame.padTop, buttonFrame.padBottom = 10, 10
    
    # Redraw button
    redrawBtn = FXButton.new(
      buttonFrame,
      "&Redraw\tRedraw the screen",
      :opts => FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT
    )
    redrawBtn.connect(SEL_COMMAND) do
      drawScene
    end
    redrawBtn.padLeft, redrawBtn.padRight = 10, 10
    redrawBtn.padTop, redrawBtn.padBottom = 5, 5
    
    # Exit button
    exitBtn = FXButton.new(
      buttonFrame,
      "&Exit\tExit the application", nil,
      getApp(),
      FXApp::ID_QUIT, 
      FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT
    )
    exitBtn.padLeft, exitBtn.padRight = 10, 10
    exitBtn.padTop, exitBtn.padBottom = 5, 5
  end

  # Create and initialize
  def create
    super
    show(PLACEMENT_SCREEN)
  end
end

if __FILE__ == $0
  application = FXApp.new("Perlin Data Viewer", "Perlin Data View")
  application.disableThreads

  # Construct the main window
  GLTestWindow.new(application)

  # Create the app's windows
  application.create

  # Run the application
  application.run
end