import os
import re

dirs = ['Engine', 'Worms2D']
for d in dirs:
    for r, _, fs in os.walk(d):
        for f in fs:
            if f.endswith('.hpp') or f.endswith('.cpp'):
                path = os.path.join(r, f)
                with open(path, 'r', encoding='utf-8') as file:
                    content = file.read()
                
                # Replace #include "../Core/..." with #include "Engine/Core/..."
                content = re.sub(r'#include "\.\./(Core|Graphics|Audio|Entities|Game)/', r'#include "Engine/\1/', content)
                
                # Replace #include "Core/..." with #include "Engine/Core/..." for files in Worms2D or Engine root
                content = re.sub(r'#include "(Core|Graphics|Audio|Entities|Game)/', r'#include "Engine/\1/', content)

                with open(path, 'w', encoding='utf-8') as file:
                    file.write(content)
