uint32_t TESV_00A3F5C0(char* Arg1_out, size_t Arg2_bufSize, const char* Arg3_inPath, const char* Arg4_prefix) {
   auto esi = Arg3_inPath;
   auto edi = Arg4_prefix;
   ebp = 0;
   auto ebx = strlen(Arg4_prefix);
   wchar ax = Arg4_prefix[0];
   auto edi = TESV_00F5871A(ecx);
   uint32_t eax = tolower(ax);
   if (eax == edi) {
      if (strncasecmp(Arg3_inPath, Arg4_prefix, ebx))
         jump to 0x00A3F662;
   } else {
      if (Arg3_inPath[0] == '\0')
         jump to 0x00A3F662;
      size_t i = 0;
      while (char c = Arg3_inPath[i++]) {
         if (ebp)
            return ebp;
         if (c != '/' && c != '\\')
            continue;
         if (ebx <= 0)
            continue;
         auto eax = strncasecmp(Arg3_inPath + i, Arg4_prefix, ebx - 1);
         if (eax)
            continue;
         //
         // const char* (Arg3_inPath + i) == substring starting with Arg4_prefix
         //
         // so given "data/meshes/abc/def" and a desired prefix "meshes/", (Arg3_inPath + i) == "meshes/abc/def"
         //
         char d = Arg3_inPath[i + ebx - 1];
         if (d == '/' || d == '\\')
            ebp = Arg3_inPath + i;
      }
   }
   // at 0x00A3F65E
   if (!ebp) {
      // at 0x00A3F662
      strcpy_s(Arg1_out, Arg2_bufSize, Arg4_prefix);
      strcat_s(Arg1_out, Arg2_bufSize, Arg3_inPath); // Arg1 = Arg4 + Arg3
      return Arg1_out;
   }
   return ebp;
}