/*
 ==============================================================================

 OpenGLDisplay.h
 Created: 22 Nov 2019 12:20:54pm
 Author:  Ross Hoyt

 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <set>
#include <map>

#define PI 3.1415926535897932384
#define NOTES_PER_OCTAVE 12
#define FIFTH 7

class OpenGLDisplay  : public Component, public MidiKeyboardStateListener, private OpenGLRenderer, private AsyncUpdater
{
public:
    OpenGLDisplay()
    {
        
        if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine(0);

        setOpaque (true);

        openGLContext.setRenderer (this);
        openGLContext.attachTo (*this);
        openGLContext.setContinuousRepainting (true);
        
        // initialize noteOnsMap
        for(int i = 0; i < 12; ++i)
        {
            noteOnsMap.insert(std::pair<int, std::set<int>>(i, std::set<int>()));
        }
    }

    ~OpenGLDisplay() override
    {
        openGLContext.detach();
    }
//    void setChordSourceArray(const std::vector<AtomicWrapper<int>> &chordSource) {
//        v_a = chordSource;
//
//    }
    void newOpenGLContextCreated() override
    {
        // nothing to do in this case - we'll initialise our shaders + textures
        // on demand, during the render callback.
        freeAllContextObjects();
    }

    void openGLContextClosing() override
    {
        // When the context is about to close, you must use this callback to delete
        // any GPU resources while the context is still current.
        freeAllContextObjects();
    }

    void freeAllContextObjects()
    {
        // noteShapes.reset(); shader.reset(); attributes.reset(); uniforms.reset(); texture.release();
    }

    void handleAsyncUpdate() override
    {
        //controlsOverlay->statusLabel.setText (statusText, dontSendNotification);
    }
    
    // This is a virtual method in OpenGLRenderer, and is called when it's time
    // to do your GL rendering.
    void renderOpenGL() override
    {
        jassert (OpenGLHelpers::isContextActive());

        auto desktopScale = (float) openGLContext.getRenderingScale();

        OpenGLHelpers::clear (Colours::black);//getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground, Colours::lightblue));

        if(true) drawBackground2DStuff (desktopScale);
        else {
            // Having used the juce 2D renderer, it will have messed-up a whole load of GL state, so
            // we need to initialise some important settings before doing our normal GL 3D drawing..
            glEnable (GL_DEPTH_TEST);
            glDepthFunc (GL_LESS);
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            openGLContext.extensions.glActiveTexture (GL_TEXTURE0);
            glEnable (GL_TEXTURE_2D);
            
            glViewport (0, 0, roundToInt (desktopScale * getWidth()), roundToInt (desktopScale * getHeight()));
            
            //texture.bind();
            
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            //  shader->use();
            
            
            //if(uniforms->translations.get() != nullptr)
            //    uniforms->translations->set(translations);
            //        for(int i = 0; i < 128; i++){
            //            if (uniforms->projectionMatrix.get() != nullptr)
            //                uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);
            //
            //
            //
            //            if (uniforms->texture.get() != nullptr)
            //                uniforms->texture->set ((GLint) 0);
            //
            //            if (uniforms->lightPosition.get() != nullptr)
            //                uniforms->lightPosition->set (-15.0f, 10.0f, 15.0f, 0.0f);
            //
            //            if (uniforms->bouncingNumber.get() != nullptr)
            //                uniforms->bouncingNumber->set (bouncingNumber.getValue());
            //
            //            if (uniforms->viewMatrix.get() != nullptr)
            //            {
            //                //Matrix3D<float> translatedViewMatrix(getViewMatrix().mat + translations[i].mat);
            //                uniforms->viewMatrix->setMatrix4 (getViewMatrix().mat, 1, false);
            //            }
            //            noteShapes->draw (openGLContext, *attributes);
            //        }
            
            // Reset the element buffers so child Components draw correctly
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }
    void paint (Graphics&) override {}

    void resized() override
    {
        //draggableOrientation.setViewport (getLocalBounds());
    }
    void drawBackground2DStuff (float desktopScale)
    {
        // Create an OpenGLGraphicsContext that will draw into this GL window..
        std::unique_ptr<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGLContext, roundToInt (desktopScale * getWidth()), roundToInt(desktopScale * getHeight())));

        if (glRenderer.get() != nullptr)
        {
            Graphics g (*glRenderer);
            g.addTransform (AffineTransform::scale (desktopScale));
            
            //drawHorizontalPitchDisplay(g);
            //drawBackroundStars(g);
            //drawCircleOfFifths2D(g);
            drawCircleOfFifthsTriangles(g);
        }
    }
    void drawCircleOfFifthsTriangles(Graphics &g)
    {
        float radius = (getWidth() > getHeight() ? getHeight() : getWidth()) / 2;
        
        
        float x1 = getWidth() / 2,  y1 = getHeight() / 2, x2,  y2,  x3,  y3;
        
        float angle = 360 / 12;
        int i = 0;
        for(int i = 0; i < NOTES_PER_OCTAVE; i++)
        {
            Path tri;
            x2 = x1 + radius * cos(degreesToRadians(i * angle));
            y2 = y1 + radius * sin(degreesToRadians(i * angle));
            x3 = x1 + radius * cos(degreesToRadians((i + 1) * angle));
            y3 = y1 + radius * sin(degreesToRadians((i + 1) * angle));
            
        
            tri.addTriangle(x1, y1, x2, y2, x3, y3);
         
            
            g.setColour(i % 2 == 0 ? Colours::seagreen : Colours::blue);
            g.fillPath(tri);
            
            if(isFundamentalPitchActive(i))
            {
                g.setColour(Colours::white);
                g.setOpacity(0.5);
                g.fillPath(tri);
            }
            //g.drawLine(getX(), getY() + i * 2, getX() + getWidth(), getY() + i * 2);
            
        }
        
    }
    void drawCircleOfFifths2D(Graphics &g)
    {
        float radius = (getWidth() > getHeight() ? getHeight() : getWidth()) / 2;
        float sliceAng = 2 * PI / NOTES_PER_OCTAVE;
        int startingLocation = 0; // 7?
        
        float circumference = 2 * PI * radius;
        float rectAng = 360 / 12;
        
        float xStart = getWidth() / 2, yStart = getHeight() / 2 + radius;
        float angle = 360 / 12;
        for(int i = 0; i < NOTES_PER_OCTAVE; ++i)
        {
            Path p;
            
            //sliceAng * startingLocation;
            
            float x = radius * cos(angle * i);//radius + sliceAng * cos(i * angle);
            float y = radius * sin(angle * i);
            
            float width = radius * cos(angle * (i + 1));
            float height = radius * sin(angle * (i + 1));
            float fromRadians = angle;//degreesToRadians(angle * i);
            float toRadians = angle;//degreesToRadians(angle * (i + 1));
            float innerCircleProportionalSize = -1;
            
            
            float toRadzz = sin(360/12 * radius);
            p.addPieSegment(x, y, width, height, fromRadians, toRadians, innerCircleProportionalSize);
            
            g.setColour( i % 2 == 0 ? Colours::white : Colours::blue);
            //g.drawLine(x,y,width,height);
            g.drawRect(x,y,width,height);
            //g.fillPath(p);
            
            startingLocation = (startingLocation + 1) % 12;
        }

    
    }
    void drawHorizontalPitchDisplay(Graphics &g)
    {
        float recWidth = getWidth() / 12, recHeight = getHeight();
        for(int i = 0; i < 12; ++i)
        {
            
            Path p;
            p.addRectangle(i * recWidth, 0,(i+1) * recWidth,recHeight);
            if(noteOnsMap.find(i)->second.size() > 0)
                g.setColour(Colours::white);
            else g.setColour(Colours::black);
            g.fillPath(p);
        }
    }
    void drawBackroundStars(Graphics &g)
    {
        for (auto s : stars)
        {
            auto size = 0.25f;
            
            // This stuff just creates a spinning star shape and fills it..
            Path p;
            p.addStar ({ getWidth()  * s.x.getValue(),
                getHeight() * s.y.getValue() },
                       7,
                       getHeight() * size * 0.5f,
                       getHeight() * size,
                       s.angle.getValue());
            
            auto hue = s.hue.getValue();
            
            g.setGradientFill (ColourGradient (Colours::green.withRotatedHue (hue).withAlpha (0.8f),
                                               0, 0,
                                               Colours::red.withRotatedHue (hue).withAlpha (0.5f),
                                               0, (float) getHeight(), false));
            g.fillPath (p);
            
        }
    }
    
    
    OpenGLContext openGLContext;
    bool draw2D = false;


    //==============================================================================
    // This is basically a sawtooth wave generator - maps a value that bounces between
    // 0.0 and 1.0 at a random speed
    struct BouncingNumber
    {
        BouncingNumber()
        : speed (0.0004 + 0.0007 * Random::getSystemRandom().nextDouble()),
        phase (Random::getSystemRandom().nextDouble())
        {
        }

        float getValue() const
        {
            double v = fmod (phase + speed * Time::getMillisecondCounterHiRes(), 2.0);
            
            return (float) (v >= 1.0 ? (2.0 - v) : v);
        }

    protected:
        double speed, phase;
    };

    struct SlowerBouncingNumber  : public BouncingNumber
    {
        SlowerBouncingNumber()
        {
            speed *= 0.3;
        }
    };
    struct BackgroundStar
    {
        SlowerBouncingNumber x, y, hue, angle;
    };
    BackgroundStar stars[3];
    
    
    // MIDI Handling
    //std::set<int> notesOn;
    std::map<int, std::set<int>> noteOnsMap;
    
    void handleNoteOn(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity ) override
    {
        //notesOn.insert(midiNoteNumber);
        //int fundamentalPitch = midiNoteNumber % 12;
        std::map<int, std::set<int>>::iterator it = noteOnsMap.find(midiNoteNumber % 12);
        if(it != noteOnsMap.end()) // probably should never be the case, could also use [] operator possibly
            it->second.insert(midiNoteNumber);
            
            
    }
    void handleNoteOff(MidiKeyboardState * source,int midiChannel, int midiNoteNumber, float velocity) override
    {
        //notesOn.erase(midiNoteNumber);
        std::map<int, std::set<int>>::iterator it = noteOnsMap.find(midiNoteNumber % 12);
        if(it != noteOnsMap.end()) // probably should never be the case, could also use [] operator possibly
            it->second.erase(midiNoteNumber);
    }
    
    bool isFundamentalPitchActive(int pitch)
    {
        return noteOnsMap.find(pitch)->second.size() > 0;
    }
    
    
};
