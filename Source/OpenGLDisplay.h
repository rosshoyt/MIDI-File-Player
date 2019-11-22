/*
 ==============================================================================

 OpenGLDisplay.h
 Created: 22 Nov 2019 12:20:54pm
 Author:  Ross Hoyt

 ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class OpenGLDisplay  : public Component, private OpenGLRenderer, private AsyncUpdater
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

    }

    ~OpenGLDisplay() override
    {
        openGLContext.detach();
    }

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

        OpenGLHelpers::clear (Colours::lightblue);//getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground, Colours::lightblue));

        drawBackground2DStuff (desktopScale);
    }
    void paint (Graphics&) override {}

    void resized() override
    {
        //draggableOrientation.setViewport (getLocalBounds());
    }
    void drawBackground2DStuff (float desktopScale)
    {
        // Create an OpenGLGraphicsContext that will draw into this GL window..
        std::unique_ptr<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGLContext,
                                                                                          roundToInt (desktopScale * getWidth()),
                                                                                          roundToInt (desktopScale * getHeight())));

        if (glRenderer.get() != nullptr)
        {
            Graphics g (*glRenderer);
            g.addTransform (AffineTransform::scale (desktopScale));

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
    }
    OpenGLContext openGLContext;



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

};
