using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Spangle
{
    class Program
    {
        static void Main(string[] args)
        {
            var spritesFile = args[0];
            var tilesFile = args[1];
            var outDir = args[2];

            var spritesImg = new Bitmap(spritesFile);
            var tilesImg = new Bitmap(tilesFile);

            var outAsm = outDir + @"data/graphics.asm";
            var outHeader = outDir + @"src/graphics.h";

            using (System.IO.StreamWriter asmFile = new System.IO.StreamWriter(outAsm))
            using (System.IO.StreamWriter headerFile = new System.IO.StreamWriter(outHeader))
            {
                asmFile.WriteLine("SECTION rodata_user");
                asmFile.WriteLine();

                WritePadding(asmFile);

                headerFile.WriteLine("#ifndef _GRAPHICS_H");
                headerFile.WriteLine("#define _GRAPHICS_H");
                headerFile.WriteLine();

                ProcessSprites(spritesImg, asmFile, headerFile, false);
                //ProcessSprites(spritesImg, asmFile, headerFile, true);

                ProcessTiles(tilesImg, asmFile, headerFile);

                headerFile.WriteLine();
                headerFile.WriteLine("#endif");
                headerFile.WriteLine();
            }
        }

        private static string Reverse(string input)
        {
            char[] charArray = input.ToCharArray();
            Array.Reverse(charArray);
            return new string(charArray);
        }

        private static void ProcessSprites(Bitmap spritesImg, System.IO.StreamWriter asmFile, System.IO.StreamWriter headerFile, bool flipH)
        {
            int sprSize = spritesImg.Height;

            int numSpr = spritesImg.Width / sprSize;
            int numCols = sprSize / 8;


            int BLACK = Color.Black.ToArgb();
            int WHITE = Color.White.ToArgb();

            if (!flipH)
            {
                headerFile.WriteLine($"#define NUM_SPR_GFX {numSpr}");
            }

            for (int i = 0; i < numSpr; ++i)
            {
                int xo = i * sprSize;
                int yo = 0;

                asmFile.WriteLine($"; Sprite {i}{(flipH ? " - fliph" : "")}");
                for (int col = 0; col < numCols; ++col)
                {
                    int colFlip = flipH ?  1-col  : col;
                    headerFile.WriteLine($"extern unsigned char spr{i}_col{col}{(flipH ? "_FH" : "")}[];");

                    asmFile.WriteLine($"PUBLIC _spr{i}_col{col}{(flipH ? "_FH" : "")}");
                    asmFile.WriteLine($"._spr{i}_col{col}{(flipH ? "_FH" : "")}");

                    for (int row = 0; row < sprSize; ++row)
                    {
                        string pix = "";
                        string mask = "";
                        for (int x = 0; x < 8; ++x)
                        {
                            int c = spritesImg.GetPixel(xo + (colFlip * 8) + x, yo + row).ToArgb();
                            if (c == BLACK)
                            {
                                pix += "1"; mask += "0";
                            }
                            else if (c == WHITE)
                            {
                                pix += "0"; mask += "0";
                            }
                            else
                            {
                                pix += "0"; mask += "1";
                            }

                        }

                        if (flipH)
                        {
                            mask = Reverse(mask);
                            pix = Reverse(pix);
                        }

                        asmFile.WriteLine($"  defb @{mask}, @{pix}");
                    }
                    asmFile.WriteLine();
                    WritePadding(asmFile);
                }
                asmFile.WriteLine();
            }
        }

        private static void ProcessTiles(Bitmap tilesImg, System.IO.StreamWriter asmFile, System.IO.StreamWriter headerFile)
        {
            int sprSize = tilesImg.Height;

            int numSpr = tilesImg.Width / sprSize;
            int numCols = sprSize / 8;

            int BLACK = Color.Black.ToArgb();
            int WHITE = Color.White.ToArgb();

            for (int i = 0; i < numSpr; ++i)
            {
                int xo = i * sprSize;
                int yo = 0;

                headerFile.WriteLine($"extern unsigned char tile{i}[];");

                asmFile.WriteLine($"PUBLIC _tile{i}");
                asmFile.WriteLine($"._tile{i}");
                for (int col = 0; col < numCols; ++col)
                {
                    for (int row = 0; row < sprSize; ++row)
                    {
                        string pix = "";
                        for (int x = 0; x < 8; ++x)
                        {
                            int c = tilesImg.GetPixel(xo + (col * 8) + x, yo + row).ToArgb();
                            if (c == BLACK)
                            {
                                pix += "1"; 
                            }
                            else 
                            {
                                pix += "0"; 
                            }
                        }

                        asmFile.WriteLine($"  defb @{pix}");
                    }
                    asmFile.WriteLine();
                }
                asmFile.WriteLine();
            }
        }

        private static void WritePadding(System.IO.StreamWriter outFile)
        {
            for (int i = 0; i < 8; ++i)
            {
                outFile.WriteLine("defb @11111111, @00000000");
            }
            outFile.WriteLine();
        }
    }
}
