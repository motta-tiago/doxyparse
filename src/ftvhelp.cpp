/******************************************************************************
 * ftvhelp.cpp,v 1.0 2000/09/06 16:09:00
 *
 * Kenney Wong <kwong@ea.com>
 *
 * Folder Tree View for offline help on browsers that do not support HTML Help.
 * Uses the FTV structure from: 
 * http://www.geocities.com/Paris/LeftBank/2178/ftexample.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <qlist.h>
#include <qdict.h>
#include <qfileinfo.h>

#include "ftvhelp.h"
#include "config.h"
#include "message.h"
#include "doxygen.h"
#include "language.h"


const char treeview_data[]=
#include "treeview.h"
;

unsigned char ftv2blank_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0f, 0x00, 0x21, 0xfe, 0x1c,
  0x55, 0x6c, 0x65, 0x61, 0x64, 0x20, 0x47, 0x49, 0x46, 0x20, 0x53, 0x6d,
  0x61, 0x72, 0x74, 0x53, 0x61, 0x76, 0x65, 0x72, 0x20, 0x56, 0x65, 0x72,
  0x20, 0x32, 0x2e, 0x30, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x16, 0x00, 0x40, 0x04, 0x14, 0xf0, 0xc9, 0x49, 0xab, 0xbd, 0x38, 0xeb,
  0xcd, 0xbb, 0xff, 0x60, 0x28, 0x8e, 0x64, 0x69, 0x9e, 0x68, 0xaa, 0x3e,
  0x11, 0x00, 0x3b
};
const unsigned int ftv2blank_gif_len = 135;

unsigned char ftv2doc_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x18, 0x00, 0x16, 0x00, 0xa2, 0x00,
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xc0,
  0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x06, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x00, 0x16, 0x00, 0x00, 0x03, 0x55, 0x68, 0xba, 0xdc,
  0xfe, 0x30, 0xca, 0x49, 0x6b, 0x24, 0x38, 0x67, 0x6b, 0x88, 0xf8, 0x60,
  0x30, 0x10, 0x96, 0x17, 0x04, 0x02, 0x2a, 0x0c, 0x02, 0x49, 0x79, 0x20,
  0x58, 0xcc, 0xc5, 0x9b, 0x02, 0x38, 0x00, 0x0e, 0xf5, 0x04, 0xc7, 0x1f,
  0x51, 0x4f, 0x62, 0xca, 0xe1, 0x3e, 0xbc, 0x17, 0x4a, 0x15, 0x4b, 0xfa,
  0x3e, 0x46, 0xdd, 0x6a, 0x78, 0x09, 0x02, 0x85, 0x4a, 0x41, 0x14, 0x49,
  0x85, 0xfc, 0x42, 0x5c, 0xdb, 0x32, 0x86, 0xf5, 0x0d, 0xce, 0xe8, 0x74,
  0xf7, 0x41, 0x6b, 0xbb, 0x39, 0xf0, 0xb8, 0xdc, 0x92, 0x00, 0x00, 0x3b
};
const unsigned int ftv2doc_gif_len = 144;

unsigned char ftv2folderclosed_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x18, 0x00, 0x16, 0x00, 0xa2, 0x00,
  0x00, 0x80, 0x00, 0x80, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0x80, 0x80,
  0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x05, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x00, 0x16, 0x00, 0x00, 0x03, 0x4c, 0x58, 0xba, 0xdc,
  0xfe, 0x30, 0xca, 0x49, 0x2b, 0x25, 0xc4, 0x3a, 0x0c, 0x40, 0xd6, 0x05,
  0xd7, 0x75, 0xdf, 0x45, 0x8c, 0xa8, 0x77, 0x09, 0x69, 0x5a, 0x42, 0x27,
  0xdb, 0x8e, 0xef, 0xd6, 0xc9, 0xb3, 0x0a, 0x8f, 0x78, 0x8b, 0xed, 0x3c,
  0x1f, 0x61, 0x20, 0x39, 0x05, 0x49, 0xc3, 0x00, 0xb1, 0x88, 0x12, 0x60,
  0x06, 0x82, 0x80, 0xb3, 0x66, 0x23, 0x0d, 0xa4, 0xd2, 0xdf, 0x85, 0x84,
  0xd5, 0x5a, 0x4e, 0x43, 0x2f, 0x08, 0x43, 0x05, 0x99, 0xcf, 0xe8, 0x49,
  0x02, 0x00, 0x3b
};
const unsigned int ftv2folderclosed_gif_len = 135;

unsigned char ftv2folderopen_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x18, 0x00, 0x16, 0x00, 0xa2, 0x00,
  0x00, 0x80, 0x00, 0x80, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xc0,
  0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x06, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x18, 0x00, 0x16, 0x00, 0x00, 0x03, 0x5f, 0x68, 0xba, 0xdc,
  0xfe, 0x30, 0xca, 0x49, 0x2b, 0x24, 0xf6, 0x0e, 0x9c, 0x59, 0x01, 0xc2,
  0xc6, 0x10, 0x24, 0xf5, 0x09, 0x02, 0x47, 0xa2, 0x9c, 0x74, 0x86, 0x2b,
  0x1a, 0x10, 0x45, 0xe1, 0x82, 0x68, 0x31, 0x08, 0x01, 0x2b, 0x00, 0x36,
  0xc8, 0x4b, 0xa0, 0x43, 0xf1, 0x08, 0xbf, 0xe0, 0x63, 0x58, 0xec, 0xa5,
  0x92, 0x11, 0xe6, 0x4e, 0x86, 0x04, 0x46, 0x71, 0xc4, 0x9d, 0xb3, 0xaa,
  0x74, 0x7c, 0x70, 0x45, 0xd6, 0xce, 0x7a, 0x05, 0x15, 0x49, 0x53, 0xf2,
  0x95, 0x88, 0x36, 0x42, 0x27, 0x5f, 0x80, 0x7c, 0x2e, 0xef, 0x46, 0x6b,
  0xf8, 0xbc, 0xbd, 0xc3, 0xef, 0xfb, 0x1b, 0x09, 0x00, 0x3b
};
const unsigned int ftv2folderopen_gif_len = 154;

unsigned char ftv2lastnode_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0f, 0x00, 0x21, 0xfe, 0x1c,
  0x55, 0x6c, 0x65, 0x61, 0x64, 0x20, 0x47, 0x49, 0x46, 0x20, 0x53, 0x6d,
  0x61, 0x72, 0x74, 0x53, 0x61, 0x76, 0x65, 0x72, 0x20, 0x56, 0x65, 0x72,
  0x20, 0x32, 0x2e, 0x30, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x16, 0x00, 0x40, 0x04, 0x1b, 0xf0, 0xc9, 0x49, 0xab, 0xbd, 0x38, 0xeb,
  0xcd, 0xbb, 0xff, 0x60, 0x28, 0x72, 0xc0, 0x57, 0x7a, 0x67, 0x97, 0x92,
  0x4f, 0xe9, 0xb6, 0x63, 0x2c, 0xcf, 0x5e, 0x04, 0x00, 0x3b
};
const unsigned int ftv2lastnode_gif_len = 142;

unsigned char ftv2link_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x18, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,
  0x80, 0x00, 0x00, 0xff, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0,
  0xc0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0f, 0x00, 0x21, 0xfe, 0x1c,
  0x55, 0x6c, 0x65, 0x61, 0x64, 0x20, 0x47, 0x49, 0x46, 0x20, 0x53, 0x6d,
  0x61, 0x72, 0x74, 0x53, 0x61, 0x76, 0x65, 0x72, 0x20, 0x56, 0x65, 0x72,
  0x20, 0x32, 0x2e, 0x30, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
  0x16, 0x00, 0x00, 0x04, 0x77, 0xf0, 0xc9, 0x49, 0xab, 0xbd, 0x38, 0xeb,
  0x7d, 0x8d, 0xff, 0x20, 0x27, 0x19, 0x93, 0x50, 0x1e, 0x24, 0x47, 0x9a,
  0x12, 0x7b, 0x3c, 0xa9, 0x16, 0xb3, 0x8f, 0x00, 0xdc, 0xc0, 0xb6, 0x3e,
  0x47, 0xe1, 0xbf, 0x82, 0x43, 0x4e, 0x26, 0xe9, 0x19, 0x0a, 0x81, 0xc2,
  0x4b, 0xa8, 0xab, 0x29, 0x0b, 0x04, 0xa4, 0x21, 0x38, 0xcc, 0xa4, 0x0a,
  0xbc, 0xc0, 0x80, 0x30, 0xe0, 0x55, 0x31, 0x2b, 0xa5, 0x21, 0xc9, 0xa5,
  0xca, 0x40, 0x06, 0x40, 0x80, 0xdb, 0x65, 0x5a, 0x2b, 0x80, 0xc1, 0x00,
  0x70, 0x30, 0xbf, 0x1f, 0x37, 0xfc, 0xed, 0xe5, 0x25, 0x02, 0xd2, 0x78,
  0x34, 0x76, 0x60, 0x78, 0x13, 0x43, 0x2e, 0x5f, 0x16, 0x31, 0x12, 0x74,
  0x07, 0x8e, 0x8e, 0x89, 0x70, 0x38, 0x93, 0x94, 0x91, 0x22, 0x97, 0x98,
  0x99, 0x9a, 0x0f, 0x11, 0x00, 0x3b
};
const unsigned int ftv2link_gif_len = 234;

unsigned char ftv2mlastnode_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x01, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x16, 0x00, 0x00, 0x04, 0x2a, 0x30, 0xc8, 0x49,
  0xab, 0x95, 0xe0, 0xea, 0x7d, 0x33, 0xff, 0x15, 0x20, 0x8e, 0x63, 0x67,
  0x5a, 0x5e, 0x78, 0xaa, 0x68, 0x20, 0xbc, 0x6f, 0x90, 0xcd, 0x53, 0x4a,
  0xd9, 0xf5, 0x7a, 0xeb, 0x35, 0x49, 0x82, 0xc0, 0xa0, 0x70, 0x48, 0x2c,
  0x1a, 0x35, 0x11, 0x00, 0x3b
};
const unsigned int ftv2mlastnode_gif_len = 125;

unsigned char ftv2mnode_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0,
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0b, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x16, 0x00, 0x00, 0x04, 0x2e, 0x70, 0xc9, 0x49,
  0xab, 0x95, 0xe0, 0xea, 0x7d, 0x33, 0xff, 0x15, 0x20, 0x8e, 0x63, 0x67,
  0x5a, 0x5e, 0x78, 0xaa, 0xe8, 0x22, 0xbc, 0xef, 0x92, 0xcd, 0x53, 0x4a,
  0xd9, 0xf5, 0x7a, 0xeb, 0x35, 0x49, 0x82, 0x40, 0x4c, 0x70, 0x28, 0x24,
  0x06, 0x71, 0xc6, 0x0d, 0x52, 0x12, 0x01, 0x00, 0x3b
};
const unsigned int ftv2mnode_gif_len = 129;

unsigned char ftv2node_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0f, 0x00, 0x21, 0xfe, 0x1c,
  0x55, 0x6c, 0x65, 0x61, 0x64, 0x20, 0x47, 0x49, 0x46, 0x20, 0x53, 0x6d,
  0x61, 0x72, 0x74, 0x53, 0x61, 0x76, 0x65, 0x72, 0x20, 0x56, 0x65, 0x72,
  0x20, 0x32, 0x2e, 0x30, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x16, 0x00, 0x40, 0x04, 0x20, 0xf0, 0xc9, 0x49, 0xab, 0xbd, 0x38, 0xeb,
  0xcd, 0xbb, 0xff, 0x60, 0x28, 0x72, 0xc0, 0x57, 0x7a, 0x67, 0x97, 0x92,
  0x4f, 0xe9, 0xb6, 0xd7, 0xba, 0xc9, 0x1a, 0x9d, 0xd9, 0x18, 0x1e, 0x63,
  0x11, 0x00, 0x3b
};
const unsigned int ftv2node_gif_len = 147;

unsigned char ftv2plastnode_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x01, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x16, 0x00, 0x00, 0x04, 0x2f, 0x30, 0xc8, 0x49,
  0xab, 0x95, 0xe0, 0xea, 0x7d, 0x33, 0xff, 0x15, 0x20, 0x8e, 0x63, 0x67,
  0x5a, 0x5e, 0x20, 0x4c, 0x29, 0x95, 0xae, 0x98, 0x29, 0xcc, 0x73, 0x90,
  0xdd, 0xec, 0x04, 0xdb, 0xa7, 0x9a, 0xa3, 0x3d, 0x17, 0x89, 0x04, 0x2a,
  0x1a, 0x8f, 0xc8, 0xa4, 0x72, 0xa9, 0x89, 0x00, 0x00, 0x3b
};
const unsigned int ftv2plastnode_gif_len = 130;

unsigned char ftv2pnode_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0,
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0b, 0x00, 0x2c, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x16, 0x00, 0x00, 0x04, 0x32, 0x70, 0xc9, 0x49,
  0xab, 0x95, 0xe0, 0xea, 0x7d, 0x33, 0xff, 0x15, 0x20, 0x8e, 0x63, 0x67,
  0x5a, 0xde, 0x22, 0x4c, 0x29, 0x95, 0xae, 0x98, 0x29, 0xcc, 0xf3, 0x92,
  0xdd, 0xec, 0x04, 0xdb, 0xa7, 0x9a, 0xa3, 0x3d, 0x17, 0x89, 0x04, 0x2a,
  0xc6, 0x8c, 0x48, 0x5e, 0xd2, 0xd8, 0x5a, 0x72, 0x9a, 0x93, 0x08, 0x00,
  0x3b
};
const unsigned int ftv2pnode_gif_len = 133;

unsigned char ftv2vertline_gif[] = {
  0x47, 0x49, 0x46, 0x38, 0x39, 0x61, 0x10, 0x00, 0x16, 0x00, 0xb3, 0x00,
  0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
  0xff, 0x21, 0xf9, 0x04, 0x01, 0x00, 0x00, 0x0f, 0x00, 0x21, 0xfe, 0x1c,
  0x55, 0x6c, 0x65, 0x61, 0x64, 0x20, 0x47, 0x49, 0x46, 0x20, 0x53, 0x6d,
  0x61, 0x72, 0x74, 0x53, 0x61, 0x76, 0x65, 0x72, 0x20, 0x56, 0x65, 0x72,
  0x20, 0x32, 0x2e, 0x30, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x16, 0x00, 0x40, 0x04, 0x19, 0xf0, 0xc9, 0x49, 0xab, 0xbd, 0x38, 0xeb,
  0xcd, 0xbb, 0xff, 0x60, 0x28, 0x72, 0xc0, 0x57, 0x7a, 0x67, 0x97, 0x92,
  0x66, 0x8b, 0xba, 0x2a, 0x5c, 0x45, 0x00, 0x3b
};
const unsigned int ftv2vertline_gif_len = 140;

struct ImageInfo
{
  const char *name;
  unsigned char *data;
  unsigned int len;
} image_info[] =
{
  { "ftv2blank.gif",ftv2blank_gif,ftv2blank_gif_len },
  { "ftv2doc.gif",ftv2doc_gif,ftv2doc_gif_len },
  { "ftv2folderclosed.gif",ftv2folderclosed_gif,ftv2folderclosed_gif_len },
  { "ftv2folderopen.gif",ftv2folderopen_gif,ftv2folderopen_gif_len },
  { "ftv2lastnode.gif",ftv2lastnode_gif,ftv2lastnode_gif_len },
  { "ftv2link.gif",ftv2link_gif,ftv2link_gif_len },
  { "ftv2mlastnode.gif",ftv2mlastnode_gif,ftv2mlastnode_gif_len },
  { "ftv2mnode.gif",ftv2mnode_gif,ftv2mnode_gif_len },
  { "ftv2node.gif",ftv2node_gif,ftv2node_gif_len },
  { "ftv2plastnode.gif",ftv2plastnode_gif,ftv2plastnode_gif_len },
  { "ftv2pnode.gif",ftv2pnode_gif,ftv2pnode_gif_len },
  { "ftv2vertline.gif",ftv2vertline_gif,ftv2vertline_gif_len },
  { 0,0,0 }
};

static void generateFolderTreeViewData()
{
  // Generate tree view script
  QCString fileName=Config_getString("HTML_OUTPUT")+"/treeview.js";
  QFile f(fileName);
  if (!f.open(IO_WriteOnly))
  {
    err("Cannot open file %s for writing!\n",fileName.data());
    return;
  }
  else
  {
    QTextStream t(&f);
    t << treeview_data;
    f.close();
  }

  // Generate alternative index.html as a frame
  fileName=Config_getString("HTML_OUTPUT")+"/index.html";
  f.setName(fileName);
  if (!f.open(IO_WriteOnly))
  {
    err("Cannot open file %s for writing!\n",fileName.data());
    return;
  }
  else
  {
    QTextStream t(&f);
#if QT_VERSION >= 200
    t.setEncoding(QTextStream::Latin1);
#endif
    t << "<html><head>";
    t << "<meta http-equiv=\"Content-Type\" content=\"text/html;charset="
      << theTranslator->idLanguageCharset() << "\">\n";
    t << "<title>"; 
    if (Config_getString("PROJECT_NAME").isEmpty())
    {
      t << "Doxygen Documentation";
    }
    else
    {
      t << Config_getString("PROJECT_NAME");
    }
    t << "</title></head>" << endl;
    t << "<frameset cols=\"" << Config_getInt("TREEVIEW_WIDTH") << ",*\">" << endl;
    t << "  <frame src=\"tree.html\" name=\"treefrm\">" << endl;
    t << "  <frame src=\"main.html\" name=\"basefrm\">" << endl;
    t << "</frameset>" << endl;
    t << "</html>" << endl;
    f.close();
  }
  
  // Generate tree view frame
  fileName=Config_getString("HTML_OUTPUT")+"/tree.html";
  f.setName(fileName);
  if (!f.open(IO_WriteOnly))
  {
    err("Cannot open file %s for writing!\n",fileName.data());
    return;
  }
  else
  {
    QTextStream t(&f);
    t << "<html><head>" << endl;
    t << "<link rel=\"stylesheet\" href=\"";
    QCString cssname=Config_getString("HTML_STYLESHEET");
    if (cssname.isEmpty())
    {
      t << "doxygen.css";
    }
    else
    {
      QFileInfo cssfi(cssname);
      if (!cssfi.exists())
      {
        err("Error: user specified HTML style sheet file does not exist!\n");
      }
      t << cssfi.fileName();
    }
    t << "\">" << endl;
    t << "<script src=\"treeview.js\"></script>" << endl;
    t << "<script src=\"tree.js\"></script>" << endl;
    t << "<script>" << endl;
    t << "initializeDocument()" << endl;
    t << "</script>" << endl;
    t << "</head>" << endl;
    t << "<body bgcolor=\"#ffffff\">" << endl;
    t << "</body>" << endl;
    t << "</html>" << endl;
    f.close();
  }

  // Generate tree view images
  ImageInfo *p = image_info;
  while (p->name)
  {
    QCString fileName=Config_getString("HTML_OUTPUT")+"/"+p->name;
    QFile f(fileName);
    if (f.open(IO_WriteOnly)) 
      f.writeBlock((char *)p->data,p->len);
    else
    {
      fprintf(stderr,"Warning: Cannot open file %s for writing\n",fileName.data());
    }
    f.close();
    p++;
  }
}

//----------------------------------------------------------------------------

FTVHelp *FTVHelp::m_theInstance = 0;

/*! Construm_cts an ftv help object. 
 *  The object has to be \link initialize() initialized\endlink before it can 
 *  be used.
 */
FTVHelp::FTVHelp()
{
  /* initial depth */
  m_dc = 0;
  m_cf = 0;
}

/*! return a reference to the one and only instance of this class. 
 */
FTVHelp *FTVHelp::getInstance()
{
  if (m_theInstance==0) m_theInstance = new FTVHelp;
  return m_theInstance;
}

/*! This will create a folder tree view table of contents file (tree.js).
 *  \sa finalize()
 */
void FTVHelp::initialize()
{
  /* open the contents file */
  QCString fName = Config_getString("HTML_OUTPUT") + "/tree.js";
  m_cf = new QFile(fName);
  if (!m_cf->open(IO_WriteOnly))
  {
    err("Could not open file %s for writing\n",fName.data());
    exit(1);
  }
  /* Write the header of the contents file */
  m_cts.setDevice(m_cf);
#if QT_VERSION >= 200
  m_cts.setEncoding(QTextStream::Latin1);
#endif

  m_cts << "foldersTree = gFld(\"<b>";
  if (Config_getString("PROJECT_NAME").isEmpty())
  {
    m_cts << "Root";
  }
  else
  {
    m_cts << Config_getString("PROJECT_NAME");
  }
  m_cts << "</b>\", \"\", \"\")\n";
}

/*! Finalizes the FTV help. This will finish and close the
 *  contents file (index.js).
 *  \sa initialize()
 */
void FTVHelp::finalize()
{
  m_cts.unsetDevice();
  m_cf->close();
  delete m_cf;
  generateFolderTreeViewData();
}

/*! Increase the level of the contents hierarchy. 
 *  This will start a new sublist in contents file.
 *  \sa decContentsDepth()
 */
int FTVHelp::incContentsDepth()
{
  //int i; for (i=0;i<m_dc+1;i++) m_cts << "  ";
  return ++m_dc;
}

/*! Decrease the level of the contents hierarchy.
 *  This will end the current sublist.
 *  \sa incContentsDepth()
 */
int FTVHelp::decContentsDepth()
{
  //int i; for (i=0;i<m_dc;i++) m_cts << "  ";
  return --m_dc;
}

/*! Add a list item to the contents file.
 *  \param isDir TRUE if the item is a directory, FALSE if it is a text
 *  \param name the name of the item.
 *  \param ref  the URL of to the item.
 */
void FTVHelp::addContentsItem(bool isDir,
                              const char *ref,
                              const char *file,
                              const char *anchor, 
                              const char *name
                             )
{
  int i; for (i=0;i<m_dc;i++) m_cts << "  ";
  QCString parent;
  QCString tagName = ref;
  QCString tagDir;
  if (ref)
  {
    tagName += ":";
    QCString *s = Doxygen::tagDestinationDict[ref];
    if (s)
    {
      tagDir = *s + "/";
      tagName += tagDir;
    }
  }
  if (m_dc==0) parent="foldersTree"; else parent.sprintf("aux%d",m_dc); 
  if (isDir) // directory entry
  {
    m_cts << "aux" << m_dc+1 << " = insFld(" << parent << ", gFld(\"" 
          << name << "\", \"" << tagName << "\", ";
    if (file)      // file optional param
    {
      m_cts << "\"" << tagDir << file << ".html\"))";
    }
    else
    {
      m_cts << "\"\"))";
    }
  }
  else // text entry
  {
    m_cts << "     insDoc(" << parent << ", gLnk(\"" 
          << name << "\", \"" << tagName << "\", ";
    if (file)      // ref optional param
    {
      m_cts << "\"" << tagDir << file << ".html";
      if (anchor) m_cts << "#" << anchor;  
      m_cts << "\"))";
    }
    else
    {
      m_cts << "\"\"))";
    }
  }
  m_cts << "\n";
}

