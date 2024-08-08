def wav_to_module(wav_file, module_file):
    with open(wav_file, 'rb') as f:
        data = f.read()
    
    with open(module_file, 'w') as f:
        f.write('export module wav_data;\n\n')
        f.write('export const vector<unsigned char> wav_data[] = {\n')
        for i, byte in enumerate(data):
            if i % 12 == 0:
                f.write('\n')
            f.write(f'0x{byte:02X}, ')
        f.write('\n};\n')

wav_to_module('D:\\SW-projects\\Solutions\\NNet\\NNetSimu\\tap.wav', 'tap_wav.ixx')