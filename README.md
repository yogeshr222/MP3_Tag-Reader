# MP3_Tag-Reader
The MP3 Tag Reader Project is a C-based utility that reads and displays metadata (ID3 tags) embedded in MP3 files. It extracts details like title, artist, album, year, and genre by parsing the ID3 header and frames, helping users view and verify song information directly from audio files.

## Resources used
### Software
1. C program, GCC compiler
2. Editor - VS Code and OS(windows/ubuntu)
### Hardware
Not required for this project

## Implementation
1. Initialization → Validates the MP3 file format and loads the file for processing.
2. Read Header → Identifies and verifies the ID3 tag header.
3. Extract Tag Frames → Reads and displays metadata fields such as Title, Artist, Album, Year, and Genre.
4. Display Data → Presents the extracted information in a user-friendly format.
5. Error Handling → Handles invalid files or missing tags gracefully.

## Conclusion
This MP3 Tag Reader project highlights how file handling, bit manipulation, and structures can be used in C to interpret binary file formats and extract meaningful information.

## Future Scope
1. Extend support for ID3v2 and other audio formats.
2. Develop a GUI interface using C++/Python for better user interaction.
   
