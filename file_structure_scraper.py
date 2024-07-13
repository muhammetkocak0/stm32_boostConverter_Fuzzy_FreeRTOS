import os

def write_directory_structure(root_dir, output_file):
    with open(output_file, 'w') as f:
        f.write(f"{os.path.basename(root_dir)}\n")
        write_structure_recursive(root_dir, f, level=0)

def write_structure_recursive(current_dir, f, level):
    items = sorted(os.listdir(current_dir))
    for i, item in enumerate(items):
        item_path = os.path.join(current_dir, item)
        if os.path.isdir(item_path):
            f.write(f"{'|   ' * level}|-- /{item}\n")
            write_structure_recursive(item_path, f, level + 1)
        else:
            f.write(f"{'|   ' * level}|-- {item}\n")

# Set the root directory to the current script's directory
root_directory = os.path.dirname(os.path.abspath(__file__))
output_file_path = os.path.join(root_directory, "directory_structure.txt")

# Call the function to write the directory structure to the text file
write_directory_structure(root_directory, output_file_path)

print(f"Directory structure of '{root_directory}' has been written to '{output_file_path}'")
