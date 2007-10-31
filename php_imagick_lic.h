/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Imagick	                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 2006-2007 Mikko Koppanen, Scott MacVicar               |
   | Imagemagick (c) ImageMagick Studio LLC                               |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   |         Scott MacVicar <scottmac@php.net>                            |
   +----------------------------------------------------------------------+
*/
#ifndef PHP_IMAGICK_LIC_H /* PHP_IMAGICK_LIC_H */
/* Prevent double inclusion */
#define PHP_IMAGICK_LIC_H

#define IMAGICK_IMAGEMAGICK_LICENSE "MS4gRGVmaW5pdGlvbnMuCgpMaWNlbnNlIHNoYWxsIG1lYW4gdGhlIHRlcm1zIGFuZCBjb25kaXRpb25zIG" \
									"ZvciB1c2UsIHJlcHJvZHVjdGlvbiwgYW5kIGRpc3RyaWJ1dGlvbiBhcyBkZWZpbmVkIGJ5IFNlY3Rpb25" \
									"zIDEgdGhyb3VnaCA5IG9mIHRoaXMgZG9jdW1lbnQuCgpMaWNlbnNvciBzaGFsbCBtZWFuIHRoZSBjb3B5" \
									"cmlnaHQgb3duZXIgb3IgZW50aXR5IGF1dGhvcml6ZWQgYnkgdGhlIGNvcHlyaWdodCBvd25lciB0aGF0I" \
									"GlzIGdyYW50aW5nIHRoZSBMaWNlbnNlLgoKTGVnYWwgRW50aXR5IHNoYWxsIG1lYW4gdGhlIHVuaW9uIG" \
									"9mIHRoZSBhY3RpbmcgZW50aXR5IGFuZCBhbGwgb3RoZXIgZW50aXRpZXMgdGhhdCBjb250cm9sLCBhcmU" \
									"gY29udHJvbGxlZCBieSwgb3IgYXJlIHVuZGVyIGNvbW1vbiBjb250cm9sIHdpdGggdGhhdCBlbnRpdHku" \
									"IEZvciB0aGUgcHVycG9zZXMgb2YgdGhpcyBkZWZpbml0aW9uLCBjb250cm9sIG1lYW5zIChpKSB0aGUgc" \
									"G93ZXIsIGRpcmVjdCBvciBpbmRpcmVjdCwgdG8gY2F1c2UgdGhlIGRpcmVjdGlvbiBvciBtYW5hZ2VtZW" \
									"50IG9mIHN1Y2ggZW50aXR5LCB3aGV0aGVyIGJ5IGNvbnRyYWN0IG9yIG90aGVyd2lzZSwgb3IgKGlpKSB" \
									"vd25lcnNoaXAgb2YgZmlmdHkgcGVyY2VudCAoNTAlKSBvciBtb3JlIG9mIHRoZSBvdXRzdGFuZGluZyBz" \
									"aGFyZXMsIG9yIChpaWkpIGJlbmVmaWNpYWwgb3duZXJzaGlwIG9mIHN1Y2ggZW50aXR5LgoKWW91IChvc" \
									"iBZb3VyKSBzaGFsbCBtZWFuIGFuIGluZGl2aWR1YWwgb3IgTGVnYWwgRW50aXR5IGV4ZXJjaXNpbmcgcG" \
									"VybWlzc2lvbnMgZ3JhbnRlZCBieSB0aGlzIExpY2Vuc2UuCgpTb3VyY2UgZm9ybSBzaGFsbCBtZWFuIHR" \
									"oZSBwcmVmZXJyZWQgZm9ybSBmb3IgbWFraW5nIG1vZGlmaWNhdGlvbnMsIGluY2x1ZGluZyBidXQgbm90" \
									"IGxpbWl0ZWQgdG8gc29mdHdhcmUgc291cmNlIGNvZGUsIGRvY3VtZW50YXRpb24gc291cmNlLCBhbmQgY" \
									"29uZmlndXJhdGlvbiBmaWxlcy4KCk9iamVjdCBmb3JtIHNoYWxsIG1lYW4gYW55IGZvcm0gcmVzdWx0aW" \
									"5nIGZyb20gbWVjaGFuaWNhbCB0cmFuc2Zvcm1hdGlvbiBvciB0cmFuc2xhdGlvbiBvZiBhIFNvdXJjZSB" \
									"mb3JtLCBpbmNsdWRpbmcgYnV0IG5vdCBsaW1pdGVkIHRvIGNvbXBpbGVkIG9iamVjdCBjb2RlLCBnZW5l" \
									"cmF0ZWQgZG9jdW1lbnRhdGlvbiwgYW5kIGNvbnZlcnNpb25zIHRvIG90aGVyIG1lZGlhIHR5cGVzLgoKV" \
									"29yayBzaGFsbCBtZWFuIHRoZSB3b3JrIG9mIGF1dGhvcnNoaXAsIHdoZXRoZXIgaW4gU291cmNlIG9yIE" \
									"9iamVjdCBmb3JtLCBtYWRlIGF2YWlsYWJsZSB1bmRlciB0aGUgTGljZW5zZSwgYXMgaW5kaWNhdGVkIGJ" \
									"5IGEgY29weXJpZ2h0IG5vdGljZSB0aGF0IGlzIGluY2x1ZGVkIGluIG9yIGF0dGFjaGVkIHRvIHRoZSB3" \
									"b3JrIChhbiBleGFtcGxlIGlzIHByb3ZpZGVkIGluIHRoZSBBcHBlbmRpeCBiZWxvdykuCgpEZXJpdmF0a" \
									"XZlIFdvcmtzIHNoYWxsIG1lYW4gYW55IHdvcmssIHdoZXRoZXIgaW4gU291cmNlIG9yIE9iamVjdCBmb3" \
									"JtLCB0aGF0IGlzIGJhc2VkIG9uIChvciBkZXJpdmVkIGZyb20pIHRoZSBXb3JrIGFuZCBmb3Igd2hpY2g" \
									"gdGhlIGVkaXRvcmlhbCByZXZpc2lvbnMsIGFubm90YXRpb25zLCBlbGFib3JhdGlvbnMsIG9yIG90aGVy" \
									"IG1vZGlmaWNhdGlvbnMgcmVwcmVzZW50LCBhcyBhIHdob2xlLCBhbiBvcmlnaW5hbCB3b3JrIG9mIGF1d" \
									"GhvcnNoaXAuIEZvciB0aGUgcHVycG9zZXMgb2YgdGhpcyBMaWNlbnNlLCBEZXJpdmF0aXZlIFdvcmtzIH" \
									"NoYWxsIG5vdCBpbmNsdWRlIHdvcmtzIHRoYXQgcmVtYWluIHNlcGFyYWJsZSBmcm9tLCBvciBtZXJlbHk" \
									"gbGluayAob3IgYmluZCBieSBuYW1lKSB0byB0aGUgaW50ZXJmYWNlcyBvZiwgdGhlIFdvcmsgYW5kIERl" \
									"cml2YXRpdmUgV29ya3MgdGhlcmVvZi4KCkNvbnRyaWJ1dGlvbiBzaGFsbCBtZWFuIGFueSB3b3JrIG9mI" \
									"GF1dGhvcnNoaXAsIGluY2x1ZGluZyB0aGUgb3JpZ2luYWwgdmVyc2lvbiBvZiB0aGUgV29yayBhbmQgYW" \
									"55IG1vZGlmaWNhdGlvbnMgb3IgYWRkaXRpb25zIHRvIHRoYXQgV29yayBvciBEZXJpdmF0aXZlIFdvcmt" \
									"zIHRoZXJlb2YsIHRoYXQgaXMgaW50ZW50aW9uYWxseSBzdWJtaXR0ZWQgdG8gTGljZW5zb3IgZm9yIGlu" \
									"Y2x1c2lvbiBpbiB0aGUgV29yayBieSB0aGUgY29weXJpZ2h0IG93bmVyIG9yIGJ5IGFuIGluZGl2aWR1Y" \
									"Wwgb3IgTGVnYWwgRW50aXR5IGF1dGhvcml6ZWQgdG8gc3VibWl0IG9uIGJlaGFsZiBvZiB0aGUgY29weX" \
									"JpZ2h0IG93bmVyLiBGb3IgdGhlIHB1cnBvc2VzIG9mIHRoaXMgZGVmaW5pdGlvbiwgc3VibWl0dGVkIG1" \
									"lYW5zIGFueSBmb3JtIG9mIGVsZWN0cm9uaWMsIHZlcmJhbCwgb3Igd3JpdHRlbiBjb21tdW5pY2F0aW9u" \
									"IGludGVudGlvbmFsbHkgc2VudCB0byB0aGUgTGljZW5zb3IgYnkgaXRzIGNvcHlyaWdodCBob2xkZXIgb" \
									"3IgaXRzIHJlcHJlc2VudGF0aXZlcywgaW5jbHVkaW5nIGJ1dCBub3QgbGltaXRlZCB0byBjb21tdW5pY2" \
									"F0aW9uIG9uIGVsZWN0cm9uaWMgbWFpbGluZyBsaXN0cywgc291cmNlIGNvZGUgY29udHJvbCBzeXN0ZW1" \
									"zLCBhbmQgaXNzdWUgdHJhY2tpbmcgc3lzdGVtcyB0aGF0IGFyZSBtYW5hZ2VkIGJ5LCBvciBvbiBiZWhh" \
									"bGYgb2YsIHRoZSBMaWNlbnNvciBmb3IgdGhlIHB1cnBvc2Ugb2YgZGlzY3Vzc2luZyBhbmQgaW1wcm92a" \
									"W5nIHRoZSBXb3JrLCBidXQgZXhjbHVkaW5nIGNvbW11bmljYXRpb24gdGhhdCBpcyBjb25zcGljdW91c2" \
									"x5IG1hcmtlZCBvciBvdGhlcndpc2UgZGVzaWduYXRlZCBpbiB3cml0aW5nIGJ5IHRoZSBjb3B5cmlnaHQ" \
									"gb3duZXIgYXMgTm90IGEgQ29udHJpYnV0aW9uLgoKQ29udHJpYnV0b3Igc2hhbGwgbWVhbiBMaWNlbnNv" \
									"ciBhbmQgYW55IGluZGl2aWR1YWwgb3IgTGVnYWwgRW50aXR5IG9uIGJlaGFsZiBvZiB3aG9tIGEgQ29ud" \
									"HJpYnV0aW9uIGhhcyBiZWVuIHJlY2VpdmVkIGJ5IExpY2Vuc29yIGFuZCBzdWJzZXF1ZW50bHkgaW5jb3" \
									"Jwb3JhdGVkIHdpdGhpbiB0aGUgV29yay4KCjIuIEdyYW50IG9mIENvcHlyaWdodCBMaWNlbnNlLiBTdWJ" \
									"qZWN0IHRvIHRoZSB0ZXJtcyBhbmQgY29uZGl0aW9ucyBvZiB0aGlzIExpY2Vuc2UsIGVhY2ggQ29udHJp" \
									"YnV0b3IgaGVyZWJ5IGdyYW50cyB0byBZb3UgYSBwZXJwZXR1YWwsIHdvcmxkd2lkZSwgbm9uLWV4Y2x1c" \
									"2l2ZSwgbm8tY2hhcmdlLCByb3lhbHR5LWZyZWUsIGlycmV2b2NhYmxlIGNvcHlyaWdodCBsaWNlbnNlIH" \
									"RvIHJlcHJvZHVjZSwgcHJlcGFyZSBEZXJpdmF0aXZlIFdvcmtzIG9mLCBwdWJsaWNseSBkaXNwbGF5LCB" \
									"wdWJsaWNseSBwZXJmb3JtLCBzdWJsaWNlbnNlLCBhbmQgZGlzdHJpYnV0ZSB0aGUgV29yayBhbmQgc3Vj" \
									"aCBEZXJpdmF0aXZlIFdvcmtzIGluIFNvdXJjZSBvciBPYmplY3QgZm9ybS4KCjMuIEdyYW50IG9mIFBhd" \
									"GVudCBMaWNlbnNlLiBTdWJqZWN0IHRvIHRoZSB0ZXJtcyBhbmQgY29uZGl0aW9ucyBvZiB0aGlzIExpY2" \
									"Vuc2UsIGVhY2ggQ29udHJpYnV0b3IgaGVyZWJ5IGdyYW50cyB0byBZb3UgYSBwZXJwZXR1YWwsIHdvcmx" \
									"kd2lkZSwgbm9uLWV4Y2x1c2l2ZSwgbm8tY2hhcmdlLCByb3lhbHR5LWZyZWUsIGlycmV2b2NhYmxlIHBh" \
									"dGVudCBsaWNlbnNlIHRvIG1ha2UsIGhhdmUgbWFkZSwgdXNlLCBvZmZlciB0byBzZWxsLCBzZWxsLCBpb" \
									"XBvcnQsIGFuZCBvdGhlcndpc2UgdHJhbnNmZXIgdGhlIFdvcmssIHdoZXJlIHN1Y2ggbGljZW5zZSBhcH" \
									"BsaWVzIG9ubHkgdG8gdGhvc2UgcGF0ZW50IGNsYWltcyBsaWNlbnNhYmxlIGJ5IHN1Y2ggQ29udHJpYnV" \
									"0b3IgdGhhdCBhcmUgbmVjZXNzYXJpbHkgaW5mcmluZ2VkIGJ5IHRoZWlyIENvbnRyaWJ1dGlvbihzKSBh" \
									"bG9uZSBvciBieSBjb21iaW5hdGlvbiBvZiB0aGVpciBDb250cmlidXRpb24ocykgd2l0aCB0aGUgV29ya" \
									"yB0byB3aGljaCBzdWNoIENvbnRyaWJ1dGlvbihzKSB3YXMgc3VibWl0dGVkLgoKNC4gUmVkaXN0cmlidX" \
									"Rpb24uIFlvdSBtYXkgcmVwcm9kdWNlIGFuZCBkaXN0cmlidXRlIGNvcGllcyBvZiB0aGUgV29yayBvciB" \
									"EZXJpdmF0aXZlIFdvcmtzIHRoZXJlb2YgaW4gYW55IG1lZGl1bSwgd2l0aCBvciB3aXRob3V0IG1vZGlm" \
									"aWNhdGlvbnMsIGFuZCBpbiBTb3VyY2Ugb3IgT2JqZWN0IGZvcm0sIHByb3ZpZGVkIHRoYXQgWW91IG1lZ" \
									"XQgdGhlIGZvbGxvd2luZyBjb25kaXRpb25zOgoKICAgMS4gWW91IG11c3QgZ2l2ZSBhbnkgb3RoZXIgcm" \
									"VjaXBpZW50cyBvZiB0aGUgV29yayBvciBEZXJpdmF0aXZlIFdvcmtzIGEgY29weSBvZiB0aGlzIExpY2V" \
									"uc2U7IGFuZAogICAyLiBZb3UgbXVzdCBjYXVzZSBhbnkgbW9kaWZpZWQgZmlsZXMgdG8gY2FycnkgcHJv" \
									"bWluZW50IG5vdGljZXMgc3RhdGluZyB0aGF0IFlvdSBjaGFuZ2VkIHRoZSBmaWxlczsgYW5kCiAgIDMuI" \
									"FlvdSBtdXN0IHJldGFpbiwgaW4gdGhlIFNvdXJjZSBmb3JtIG9mIGFueSBEZXJpdmF0aXZlIFdvcmtzIH" \
									"RoYXQgWW91IGRpc3RyaWJ1dGUsIGFsbCBjb3B5cmlnaHQsIHBhdGVudCwgdHJhZGVtYXJrLCBhbmQgYXR" \
									"0cmlidXRpb24gbm90aWNlcyBmcm9tIHRoZSBTb3VyY2UgZm9ybSBvZiB0aGUgV29yaywgZXhjbHVkaW5n" \
									"IHRob3NlIG5vdGljZXMgdGhhdCBkbyBub3QgcGVydGFpbiB0byBhbnkgcGFydCBvZiB0aGUgRGVyaXZhd" \
									"Gl2ZSBXb3JrczsgYW5kCiAgIDQuIElmIHRoZSBXb3JrIGluY2x1ZGVzIGEgTk9USUNFIHRleHQgZmlsZS" \
									"BhcyBwYXJ0IG9mIGl0cyBkaXN0cmlidXRpb24sIHRoZW4gYW55IERlcml2YXRpdmUgV29ya3MgdGhhdCB" \
									"Zb3UgZGlzdHJpYnV0ZSBtdXN0IGluY2x1ZGUgYSByZWFkYWJsZSBjb3B5IG9mIHRoZSBhdHRyaWJ1dGlv" \
									"biBub3RpY2VzIGNvbnRhaW5lZCB3aXRoaW4gc3VjaCBOT1RJQ0UgZmlsZSwgZXhjbHVkaW5nIHRob3NlI" \
									"G5vdGljZXMgdGhhdCBkbyBub3QgcGVydGFpbiB0byBhbnkgcGFydCBvZiB0aGUgRGVyaXZhdGl2ZSBXb3" \
									"JrcywgaW4gYXQgbGVhc3Qgb25lIG9mIHRoZSBmb2xsb3dpbmcgcGxhY2VzOiB3aXRoaW4gYSBOT1RJQ0U" \
									"gdGV4dCBmaWxlIGRpc3RyaWJ1dGVkIGFzIHBhcnQgb2YgdGhlIERlcml2YXRpdmUgV29ya3M7IHdpdGhp" \
									"biB0aGUgU291cmNlIGZvcm0gb3IgZG9jdW1lbnRhdGlvbiwgaWYgcHJvdmlkZWQgYWxvbmcgd2l0aCB0a" \
									"GUgRGVyaXZhdGl2ZSBXb3Jrczsgb3IsIHdpdGhpbiBhIGRpc3BsYXkgZ2VuZXJhdGVkIGJ5IHRoZSBEZX" \
									"JpdmF0aXZlIFdvcmtzLCBpZiBhbmQgd2hlcmV2ZXIgc3VjaCB0aGlyZC1wYXJ0eSBub3RpY2VzIG5vcm1" \
									"hbGx5IGFwcGVhci4gVGhlIGNvbnRlbnRzIG9mIHRoZSBOT1RJQ0UgZmlsZSBhcmUgZm9yIGluZm9ybWF0" \
									"aW9uYWwgcHVycG9zZXMgb25seSBhbmQgZG8gbm90IG1vZGlmeSB0aGUgTGljZW5zZS4gWW91IG1heSBhZ" \
									"GQgWW91ciBvd24gYXR0cmlidXRpb24gbm90aWNlcyB3aXRoaW4gRGVyaXZhdGl2ZSBXb3JrcyB0aGF0IF" \
									"lvdSBkaXN0cmlidXRlLCBhbG9uZ3NpZGUgb3IgYXMgYW4gYWRkZW5kdW0gdG8gdGhlIE5PVElDRSB0ZXh" \
									"0IGZyb20gdGhlIFdvcmssIHByb3ZpZGVkIHRoYXQgc3VjaCBhZGRpdGlvbmFsIGF0dHJpYnV0aW9uIG5v" \
									"dGljZXMgY2Fubm90IGJlIGNvbnN0cnVlZCBhcyBtb2RpZnlpbmcgdGhlIExpY2Vuc2UuCgpZb3UgbWF5I" \
									"GFkZCBZb3VyIG93biBjb3B5cmlnaHQgc3RhdGVtZW50IHRvIFlvdXIgbW9kaWZpY2F0aW9ucyBhbmQgbW" \
									"F5IHByb3ZpZGUgYWRkaXRpb25hbCBvciBkaWZmZXJlbnQgbGljZW5zZSB0ZXJtcyBhbmQgY29uZGl0aW9" \
									"ucyBmb3IgdXNlLCByZXByb2R1Y3Rpb24sIG9yIGRpc3RyaWJ1dGlvbiBvZiBZb3VyIG1vZGlmaWNhdGlv" \
									"bnMsIG9yIGZvciBhbnkgc3VjaCBEZXJpdmF0aXZlIFdvcmtzIGFzIGEgd2hvbGUsIHByb3ZpZGVkIFlvd" \
									"XIgdXNlLCByZXByb2R1Y3Rpb24sIGFuZCBkaXN0cmlidXRpb24gb2YgdGhlIFdvcmsgb3RoZXJ3aXNlIG" \
									"NvbXBsaWVzIHdpdGggdGhlIGNvbmRpdGlvbnMgc3RhdGVkIGluIHRoaXMgTGljZW5zZS4KCjUuIFN1Ym1" \
									"pc3Npb24gb2YgQ29udHJpYnV0aW9ucy4gVW5sZXNzIFlvdSBleHBsaWNpdGx5IHN0YXRlIG90aGVyd2lz" \
									"ZSwgYW55IENvbnRyaWJ1dGlvbiBpbnRlbnRpb25hbGx5IHN1Ym1pdHRlZCBmb3IgaW5jbHVzaW9uIGluI" \
									"HRoZSBXb3JrIGJ5IFlvdSB0byB0aGUgTGljZW5zb3Igc2hhbGwgYmUgdW5kZXIgdGhlIHRlcm1zIGFuZC" \
									"Bjb25kaXRpb25zIG9mIHRoaXMgTGljZW5zZSwgd2l0aG91dCBhbnkgYWRkaXRpb25hbCB0ZXJtcyBvciB" \
									"jb25kaXRpb25zLiBOb3R3aXRoc3RhbmRpbmcgdGhlIGFib3ZlLCBub3RoaW5nIGhlcmVpbiBzaGFsbCBz" \
									"dXBlcnNlZGUgb3IgbW9kaWZ5IHRoZSB0ZXJtcyBvZiBhbnkgc2VwYXJhdGUgbGljZW5zZSBhZ3JlZW1lb" \
									"nQgeW91IG1heSBoYXZlIGV4ZWN1dGVkIHdpdGggTGljZW5zb3IgcmVnYXJkaW5nIHN1Y2ggQ29udHJpYn" \
									"V0aW9ucy4KCjYuIFRyYWRlbWFya3MuIFRoaXMgTGljZW5zZSBkb2VzIG5vdCBncmFudCBwZXJtaXNzaW9" \
									"uIHRvIHVzZSB0aGUgdHJhZGUgbmFtZXMsIHRyYWRlbWFya3MsIHNlcnZpY2UgbWFya3MsIG9yIHByb2R1" \
									"Y3QgbmFtZXMgb2YgdGhlIExpY2Vuc29yLCBleGNlcHQgYXMgcmVxdWlyZWQgZm9yIHJlYXNvbmFibGUgY" \
									"W5kIGN1c3RvbWFyeSB1c2UgaW4gZGVzY3JpYmluZyB0aGUgb3JpZ2luIG9mIHRoZSBXb3JrIGFuZCByZX" \
									"Byb2R1Y2luZyB0aGUgY29udGVudCBvZiB0aGUgTk9USUNFIGZpbGUuCgo3LiBEaXNjbGFpbWVyIG9mIFd" \
									"hcnJhbnR5LiBVbmxlc3MgcmVxdWlyZWQgYnkgYXBwbGljYWJsZSBsYXcgb3IgYWdyZWVkIHRvIGluIHdy" \
									"aXRpbmcsIExpY2Vuc29yIHByb3ZpZGVzIHRoZSBXb3JrIChhbmQgZWFjaCBDb250cmlidXRvciBwcm92a" \
									"WRlcyBpdHMgQ29udHJpYnV0aW9ucykgb24gYW4gQVMgSVMgQkFTSVMsIFdJVEhPVVQgV0FSUkFOVElFUy" \
									"BPUiBDT05ESVRJT05TIE9GIEFOWSBLSU5ELCBlaXRoZXIgZXhwcmVzcyBvciBpbXBsaWVkLCBpbmNsdWR" \
									"pbmcsIHdpdGhvdXQgbGltaXRhdGlvbiwgYW55IHdhcnJhbnRpZXMgb3IgY29uZGl0aW9ucyBvZiBUSVRM" \
									"RSwgTk9OLUlORlJJTkdFTUVOVCwgTUVSQ0hBTlRBQklMSVRZLCBvciBGSVRORVNTIEZPUiBBIFBBUlRJQ" \
									"1VMQVIgUFVSUE9TRS4gWW91IGFyZSBzb2xlbHkgcmVzcG9uc2libGUgZm9yIGRldGVybWluaW5nIHRoZS" \
									"BhcHByb3ByaWF0ZW5lc3Mgb2YgdXNpbmcgb3IgcmVkaXN0cmlidXRpbmcgdGhlIFdvcmsgYW5kIGFzc3V" \
									"tZSBhbnkgcmlza3MgYXNzb2NpYXRlZCB3aXRoIFlvdXIgZXhlcmNpc2Ugb2YgcGVybWlzc2lvbnMgdW5k" \
									"ZXIgdGhpcyBMaWNlbnNlLgoKOC4gTGltaXRhdGlvbiBvZiBMaWFiaWxpdHkuIEluIG5vIGV2ZW50IGFuZ" \
									"CB1bmRlciBubyBsZWdhbCB0aGVvcnksIHdoZXRoZXIgaW4gdG9ydCAoaW5jbHVkaW5nIG5lZ2xpZ2VuY2" \
									"UpLCBjb250cmFjdCwgb3Igb3RoZXJ3aXNlLCB1bmxlc3MgcmVxdWlyZWQgYnkgYXBwbGljYWJsZSBsYXc" \
									"gKHN1Y2ggYXMgZGVsaWJlcmF0ZSBhbmQgZ3Jvc3NseSBuZWdsaWdlbnQgYWN0cykgb3IgYWdyZWVkIHRv" \
									"IGluIHdyaXRpbmcsIHNoYWxsIGFueSBDb250cmlidXRvciBiZSBsaWFibGUgdG8gWW91IGZvciBkYW1hZ" \
									"2VzLCBpbmNsdWRpbmcgYW55IGRpcmVjdCwgaW5kaXJlY3QsIHNwZWNpYWwsIGluY2lkZW50YWwsIG9yIG" \
									"NvbnNlcXVlbnRpYWwgZGFtYWdlcyBvZiBhbnkgY2hhcmFjdGVyIGFyaXNpbmcgYXMgYSByZXN1bHQgb2Y" \
									"gdGhpcyBMaWNlbnNlIG9yIG91dCBvZiB0aGUgdXNlIG9yIGluYWJpbGl0eSB0byB1c2UgdGhlIFdvcmsg" \
									"KGluY2x1ZGluZyBidXQgbm90IGxpbWl0ZWQgdG8gZGFtYWdlcyBmb3IgbG9zcyBvZiBnb29kd2lsbCwgd" \
									"29yayBzdG9wcGFnZSwgY29tcHV0ZXIgZmFpbHVyZSBvciBtYWxmdW5jdGlvbiwgb3IgYW55IGFuZCBhbG" \
									"wgb3RoZXIgY29tbWVyY2lhbCBkYW1hZ2VzIG9yIGxvc3NlcyksIGV2ZW4gaWYgc3VjaCBDb250cmlidXR" \
									"vciBoYXMgYmVlbiBhZHZpc2VkIG9mIHRoZSBwb3NzaWJpbGl0eSBvZiBzdWNoIGRhbWFnZXMuCgo5LiBB" \
									"Y2NlcHRpbmcgV2FycmFudHkgb3IgQWRkaXRpb25hbCBMaWFiaWxpdHkuIFdoaWxlIHJlZGlzdHJpYnV0a" \
									"W5nIHRoZSBXb3JrIG9yIERlcml2YXRpdmUgV29ya3MgdGhlcmVvZiwgWW91IG1heSBjaG9vc2UgdG8gb2" \
									"ZmZXIsIGFuZCBjaGFyZ2UgYSBmZWUgZm9yLCBhY2NlcHRhbmNlIG9mIHN1cHBvcnQsIHdhcnJhbnR5LCB" \
									"pbmRlbW5pdHksIG9yIG90aGVyIGxpYWJpbGl0eSBvYmxpZ2F0aW9ucyBhbmQvb3IgcmlnaHRzIGNvbnNp" \
									"c3RlbnQgd2l0aCB0aGlzIExpY2Vuc2Uu"

#endif /* PHP_IMAGICK_LIC_H */
