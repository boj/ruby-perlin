require 'perlin'
require 'rubygems'
require 'fox16'

# Base framework borrowed from: http://www.fxruby.org/examples/gltest.rb

include Fox

class GLTestWindow < FXMainWindow

  # How often our timer will fire (in milliseconds)
  TIMER_INTERVAL = 100
  
  def drawTerrain(width, length)
    p = Perlin.new(@p_seed.to_s.to_i, @p_freq.to_s.to_f, @p_octave.to_s.to_i)
    
    GL.Begin(GL::QUADS)
    for x in 0..width
      for y in 0..length
        material = [rand, rand, rand]
        GL.Material(GL::FRONT, GL::AMBIENT, material)
        GL.Material(GL::FRONT, GL::DIFFUSE, material)
        GL.Vertex(x, y, p.run(x, y))
        GL.Vertex(x + 1, y, p.run(x + 1, y))
        GL.Vertex(x + 1, y + 1, p.run(x + 1, y + 1))
        GL.Vertex(x, y + 1, p.run(x, y + 1))
      end
    end
    GL.End()
  end

  # Draw the GL scene
  def drawScene
    lightPosition = [5.0, 5.0, 5.0, 1.0]
    lightAmbient  = [ 0.5,  0.5, 0.5, 1.0]
    lightDiffuse  = [ 0.9,  0.9, 0.9, 1.0]
  
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
    x = (@p_width.to_s.to_i / 2.0)
    y = (@p_length.to_s.to_i / 2.0)
    GLU.LookAt(5.0, 15.0, 5.0, x, y, 0.0, 0.0, -1.0, 0.0)
  
    GL.ShadeModel(GL::SMOOTH)
    GL.Light(GL::LIGHT0, GL::POSITION, lightPosition)
    GL.Light(GL::LIGHT0, GL::AMBIENT, lightAmbient)
    GL.Light(GL::LIGHT0, GL::DIFFUSE, lightDiffuse)
    GL.Enable(GL::LIGHT0)
    GL.Enable(GL::LIGHTING)
  
    GL.PushMatrix()
    drawTerrain(@p_width.to_s.to_i, @p_length.to_s.to_i)
    GL.PopMatrix()
  
    # Swap if it is double-buffered
    if @glvisual.isDoubleBuffer
      @glcanvas.swapBuffers
    end
    
    # Make context non-current
    @glcanvas.makeNonCurrent
  end

  def initialize(app)
    @p_seed   = FXDataTarget.new(rand(100))
    @p_freq   = FXDataTarget.new(1.0)
    @p_octave = FXDataTarget.new(1)
    @p_width  = FXDataTarget.new(10)
    @p_length = FXDataTarget.new(10)
    
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
      x = (@p_width.to_s.to_i / 2.0)
      y = (@p_length.to_s.to_i / 2.0)
      GLU.LookAt(0.0, 0.0, 10.0, x, y, 0.0, 0.0, 1.0, 0.0)
      drawScene
    end
    redrawBtn.padLeft, redrawBtn.padRight = 10, 10
    redrawBtn.padTop, redrawBtn.padBottom = 5, 5
    
    # Redraw button
    redrawRndBtn = FXButton.new(
      buttonFrame,
      "&Redraw Random\tRedraw the screen after changing the seed",
      :opts => FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_TOP | LAYOUT_LEFT
    )
    redrawRndBtn.connect(SEL_COMMAND) do
      @p_seed   = FXDataTarget.new(rand(100))
      drawScene
    end
    redrawRndBtn.padLeft, redrawRndBtn.padRight = 10, 10
    redrawRndBtn.padTop, redrawRndBtn.padBottom = 5, 5
    
    FXLabel.new(buttonFrame, "Seed", nil, LAYOUT_CENTER_Y | LAYOUT_CENTER_X | JUSTIFY_RIGHT | LAYOUT_FILL_ROW)
    FXTextField.new(buttonFrame, 10, @p_seed, FXDataTarget::ID_VALUE, TEXTFIELD_INTEGER | LAYOUT_FILL_ROW)
    
    FXLabel.new(buttonFrame, "Frequency", nil, LAYOUT_CENTER_Y | LAYOUT_CENTER_X | JUSTIFY_RIGHT | LAYOUT_FILL_ROW)
    FXTextField.new(buttonFrame, 10, @p_freq, FXDataTarget::ID_VALUE, TEXTFIELD_REAL | LAYOUT_FILL_ROW)
    
    FXLabel.new(buttonFrame, "Octave", nil, LAYOUT_CENTER_Y | LAYOUT_CENTER_X | JUSTIFY_RIGHT | LAYOUT_FILL_ROW)
    FXTextField.new(buttonFrame, 10, @p_octave, FXDataTarget::ID_VALUE, TEXTFIELD_INTEGER | LAYOUT_FILL_ROW)
    
    FXLabel.new(buttonFrame, "Width", nil, LAYOUT_CENTER_Y | LAYOUT_CENTER_X | JUSTIFY_RIGHT | LAYOUT_FILL_ROW)
    FXTextField.new(buttonFrame, 10, @p_width, FXDataTarget::ID_VALUE, TEXTFIELD_INTEGER | LAYOUT_FILL_ROW)
    
    FXLabel.new(buttonFrame, "Length", nil, LAYOUT_CENTER_Y | LAYOUT_CENTER_X | JUSTIFY_RIGHT | LAYOUT_FILL_ROW)
    FXTextField.new(buttonFrame, 10, @p_length, FXDataTarget::ID_VALUE, TEXTFIELD_INTEGER | LAYOUT_FILL_ROW)
    
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