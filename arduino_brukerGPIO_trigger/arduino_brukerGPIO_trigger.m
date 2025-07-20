function []= arduino_brukerGPIO_trigger(device, duration, pulse_width, arduino_onset_delay, brukerGPIO_onset_delay, sample_rate, periodic_freq)
%% Generates a start and stop 5V TTL trigger for arduino and brukerGPIO(which is used by prairie view)
% state: boolean, set to 1 inorder to trigger arduino and brukerGPIO, else
% 0 to only trigger bruker_GPIO (equivalent of stimulation/no stimulation
% data mode)
% 
% duration: total duration between start and stop trigger pulses in sec
% pulse_width: width of pulse in sec
    ANALOG_HIGH = 5;
    %create NI DAQ session
    daq_session = daq.createSession('ni');
    daq_session.Rate = sample_rate; %inline with arduino frequency(Hz)
    %how to add digital channel??
    extend_duration = 0;
    switch device
        case {'arduino','stimulus_test'}
            daq_session.addAnalogOutputChannel('dev2','ao1', 'Voltage');   %arduino analog
            arduino_analog_data_stream = ANALOG_HIGH*generate_data_stream('start_stop', duration, pulse_width, arduino_onset_delay, sample_rate, extend_duration);
            daq_session.queueOutputData([arduino_analog_data_stream']);
        case {'bruker_GPIO','control', 'imaging_test', 'imaging_control'}
            daq_session.addAnalogOutputChannel('dev2','ao0', 'Voltage');   %brukerGPIO
            brukerGPIO_data_stream = generate_data_stream('start_stop', duration, pulse_width, brukerGPIO_onset_delay, sample_rate, extend_duration);
            daq_session.queueOutputData([brukerGPIO_data_stream']);
        case {'both', 'stimulus', 'imaging_stimulus'}
            daq_session.addAnalogOutputChannel('dev2','ao0', 'Voltage');   %brukerGPIO
            daq_session.addAnalogOutputChannel('dev2','ao1', 'Voltage');   %arduino analog
            brukerGPIO_data_stream = generate_data_stream('start_stop', duration, pulse_width, brukerGPIO_onset_delay, sample_rate, extend_duration);
            arduino_analog_data_stream = ANALOG_HIGH*generate_data_stream('start_stop', duration, pulse_width, arduino_onset_delay, sample_rate, extend_duration);
            daq_session.queueOutputData([brukerGPIO_data_stream', arduino_analog_data_stream']);
        case {'imaging_stimulus_periodic', 'stimulus_peridoic'}
            daq_session.addAnalogOutputChannel('dev2','ao0', 'Voltage');   %brukerGPIO
            daq_session.addAnalogOutputChannel('dev2','ao1', 'Voltage');   %arduino analog
            brukerGPIO_data_stream = generate_data_stream('start_stop', duration, pulse_width, brukerGPIO_onset_delay, sample_rate, extend_duration);
            arduino_analog_data_stream = ANALOG_HIGH*generate_data_stream('periodic', duration, pulse_width, arduino_onset_delay, sample_rate, extend_duration, periodic_freq);
            daq_session.queueOutputData([brukerGPIO_data_stream', arduino_analog_data_stream']);
        case {'reset', 'offset'}
            daq_session.addAnalogOutputChannel('dev2','ao1', 'Voltage');   %arduino analog
            arduino_analog_data_stream = ANALOG_HIGH*generate_data_stream('single',duration, pulse_width, arduino_onset_delay, sample_rate, extend_duration);
            daq_session.queueOutputData(arduino_analog_data_stream');
    end
    %arduino_digital_data_stream = state*generate_data_stream(duration, pulse_width, arduino_onset_delay, sample_rate, extend_duration);
    %figure
    %title('brukerGPIO_trigger')
    %xlabel('time')
    %ylabel('Voltage')
    %plot((1:length(brukerGPIO_data_stream))/sample_rate, brukerGPIO_data_stream,'g');
    %hold on
    %plot((1:length(arduino_data_stream))/sample_rate, arduino_data_stream,'r');
    %hold on
    %daq_session.queueOutputData([brukerGPIO_data_stream', arduino_analog_data_stream', arduino_digital_data_stream']);
    daq_session.startForeground;

end

function [data_stream] = generate_data_stream(type, duration, pulse_width, onset_delay, sample_rate, extend_duration, period_length, period_gap)
%% 
% type: 'start_stop' trigger_pulse, 'single' trigger pulse
% duration: total duration between start and stop trigger pulses
% pulse_width: width of pulse in sec
% onset_delay: delay for trigger onset. Note: if greater than 0, this will be added to the
% total length of data stream in addition to total_duration length
% sample_rate: the sample rate set for NI DAQ hardware
% extend_duration: 1 or 0, extend total duration of the data_stream if set to 1
    HIGH = 1;
    LOW = 0;
    switch type
        case 'single'
            pulse_length = floor(pulse_width*sample_rate);
            stream_length = floor((duration+extend_duration*onset_delay)*sample_rate + pulse_length) + 1;

            start_trigger_loc = floor(1 + onset_delay*sample_rate);
            data_stream = LOW*zeros(1, stream_length);
            data_stream(start_trigger_loc:(start_trigger_loc+pulse_length)) = HIGH;
            data_stream(end) = LOW;
        case 'start_stop'
            pulse_length = floor(pulse_width*sample_rate);
            stream_length = floor((duration+extend_duration*onset_delay)*sample_rate + 2*pulse_length)+1;

            start_trigger_loc = floor(1 + onset_delay*sample_rate);
            stop_trigger_loc = stream_length-pulse_length-5;

            data_stream = LOW*zeros(1, stream_length);
            data_stream(start_trigger_loc:(start_trigger_loc+pulse_length)) = HIGH;
            data_stream(stop_trigger_loc:(stop_trigger_loc+pulse_length)) = HIGH;
            data_stream(end) = LOW;
        case 'periodic'
            error('Incomplete function implementation for periodic');
            pulse_length = floor(pulse_width*sample_rate);
            periodic_stream_length = generate_data_stream('single',period_length, 0.0005, 0, sample_rate, 0);
            n_repeats = duration
            stream_length = floor((duration+extend_duration*onset_delay)*sample_rate + 2*n_repeats*pulse_length) + 1;
      
            if nargin == 7
                
            else
                
            end
            
    end
    
end
