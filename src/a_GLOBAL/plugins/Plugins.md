# Plugins

## Existing plugins

### LED HEX

There are 2 HATs that already work with the provided `LED_HEX.h` plugin. To make use of this plugin, remove the comment marks `//` from the lines in `k_PLUGINMANAGER.h`.

##### HEX HAT

You don't have to change the `LED_HEX.h` file. 🎉

##### NEOFLASH HAT

Change the `NUM_LEDS` variable to `126`
Change the `DATA_PIN` variable to `26`

---

## Develop a plugin

To make use of the HATs that M5Stack develops, I've created a way to develop plugins, so that you can add functionality to the stick if you want to. Create a file in the `/plugins` directory, and make sure it's a `.h` file. As an example, we'll call our example plugin: `Example.h`.

### Basic plugin

```C++
class Example{

    public:
        Example(){

        }

        void onLive() {

        }

        void onPre() {

        }

        void onSafe() {

        }

        void onClear() {

        }
};
```

It's pretty self-explanatory: The `onLive`, `onPre`, `onSafe` and `onClear` methods are called whenever the state of the tally changes.
When it switches to LIVE, the `onLive` function is called, the same counts for the other functions.

### Hook into the code

When you created a plugin, you need to open the `k_PLUGINMANAGER.h` file, and initialize the plugin class you've created:

```
#include "plugins/Example.h";

class PluginManager {
  public:
    PluginManager(){}

    Example examplePlugin;
    void onLive(){
      examplePlugin.onLive();
    }

    void onPre() {
      examplePlugin.onPre();
    }

    void onSafe() {
        examplePlugin.onSafe();
    }
};
```

Do this for every plugin you have. I know, it's not ideal. I'm still looking for a good and more efficient alternative to add plugins, but this is what I have so far... Suggestions are more than welcome 😄

If something is not clear, please feel free to report an issue, so that I can improve this documentation.
