import Module from './maxi-audio.wasmmodule.js';

/**
 * The main Maxi Audio wrapper with a WASM-powered AudioWorkletProcessor.
 *
 * @class MaxiAudioProcessor
 * @extends AudioWorkletProcessor
 */
class MaxiAudioProcessor extends AudioWorkletProcessor {

  static get parameterDescriptors() {
    return [{ name: 'gain', defaultValue: 0.1 }];
  }

  /**
   * @constructor
   */
  constructor() {
    super();
    this.sampleRate = 44100;

    this.port.onmessage = (event) => {
      console.log(event.data);
    };

    this.mySine = new Module.maxiOsc();
    this.myOtherSine = new Module.maxiOsc();
  }

  /**
   * @process
   */
  process(inputs, outputs, parameters) {

    const outputsLength = outputs.length;
    for (let outputId = 0; outputId < outputsLength; ++outputId) {
      let output = outputs[outputId];
      let input = inputs[outputId];
      const channelLenght = output.length;

      for (let channelId = 0; channelId < channelLenght; ++channelId) {
        const gain = parameters.gain;
        const isConstant = gain.length === 1
        let inputChannel = input[channelId];
        let outputChannel = output[channelId];

        for (let i = 0; i < outputChannel.length; ++i) {
          const amp = isConstant ? gain[0] : gain[i]
          // outputChannel[i] = (this.mySine.sawn(60) * this.myOtherSine.sinewave(0.4)) * amp;
          let mic = inputChannel[i];
          outputChannel[i] = mic * (this.myOtherSine.sinewave(900)) * amp;
        }
      }
    }
    return true;
  }

};

registerProcessor("maxi-audio-processor", MaxiAudioProcessor);