def to_module(input_file, module_file):
    with open(input_file, 'rb') as f:
        data = f.read()
    
    with open(module_file, 'w') as f:
        f.write('export module input_data;\n\n')
        f.write('export const vector<unsigned char> input_data = {\n')
        for i, byte in enumerate(data):
            if i % 12 == 0:
                f.write('\n')
            f.write(f'0x{byte:02X}, ')
        f.write('\n};\n')

to_module('icon-redo.ico', 'icon-redo.txt')