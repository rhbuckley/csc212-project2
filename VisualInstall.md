## Instructions

### Installation of SDL2 for SDL Visualization

#### Steps on macOS:

Go to the SDL website: https://www.libsdl.org/

Click SDL Releases under Downloads on the left side of the website, this will direct you to SDL’s latest release on GitHub

Click the download that is correct for your computer

To skip this step click the following for Macbooks: https://www.google.com/url?q=https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/SDL2-2.28.1.dmg&sa=D&source=docs&ust=1690251149138911&usg=AOvVaw2SU3_hxGrFtx0p9yjDeuYV

Clicking here will download the correct file

From here, open a new finder window by right clicking the finder icon and selecting New Finder Window near the top

On the top of your screen, select Go

Then, select Go to Folder

This will bring up a text box saying Go to Folder, click into this and paste the following: /Library/Frameworks

Hit enter, then you will see the Frameworks folder on your Macbook

Go back to the open SDL2 file and drag the framework into the Frameworks folder

For these next few steps, I am assuming that you have the compiler g++ installed as well as an IDE

Open a new file and copy in the code located here

You will see lots of errors, this is normal and to be expected from doing it this way because the install of SDL2 is not

completely linked to your IDE.

Now, you are ready to compile

When compiling, use this format, replacing the file name with the file of your choosing

```
g++ <file_name>.cpp -o program -L /Library/Frameworks/SDL2.framework/Headers -F /Library/Frameworks/ -framework SDL2
```

With our project, use the following line:

```
g++  visual.cpp -o program -L /Library/Frameworks/SDL2.framework/Headers -F /Library/Frameworks/ -framework SDL2
```

Once compiled, enter the following line into your terminal with the correct input file name and the correct mode:

```
./program <file_name>.txt <mode>
```

There are four text files in the GitHub to be tested

You can use the following modes:

      0: Insertion Sort

      1: Merge Sort

      2: Quicksort

      3: Radix Sort

Now, after running, a separate window will open, showing a sort occuring
