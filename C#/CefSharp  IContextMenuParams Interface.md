# CefSharp     IContextMenuParams Interface

**Namespace:** [CefSharp](https://cefsharp.github.io/api/57.0.0/html/T_CefSharp_IContextMenuParams.htm)
**Assembly:** CefSharp (in CefSharp.dll) Version: 57.0.0.0 (57.0.0.0)

## Properties

|   | Name | Description |
| - | - | - |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [DictionarySuggestions](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_DictionarySuggestions.htm) | Returns a list of strings from the spell check service for the misspelled word if there is one. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [EditStateFlags](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_EditStateFlags.htm) | Returns flags representing the actions supported by the editable node, if any, that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [FrameCharset](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_FrameCharset.htm) | Returns the character encoding of the subframe that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [FrameUrl](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_FrameUrl.htm) | Returns the URL of the subframe that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [HasImageContents](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_HasImageContents.htm) | Returns true if the context menu was invoked on an image which has non-empty contents. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [IsCustomMenu](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_IsCustomMenu.htm) | Returns true if the context menu contains items specified by the renderer process (for example, plugin placeholder or pepper plugin menu items). |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [IsDisposed](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_IsDisposed.htm) | Gets a value indicating whether the object has been disposed of. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [IsEditable](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_IsEditable.htm) | Returns true if the context menu was invoked on an editable node. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [IsPepperMenu](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_IsPepperMenu.htm) | Returns true if the context menu was invoked from a pepper plugin. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [IsSpellCheckEnabled](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_IsSpellCheckEnabled.htm) | Returns true if the context menu was invoked on an editable node where spell-check is enabled. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [LinkUrl](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_LinkUrl.htm) | Returns the URL of the link, if any, that encloses the node that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [MediaStateFlags](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_MediaStateFlags.htm) | Returns flags representing the actions supported by the media element, if any, that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [MediaType](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_MediaType.htm) | Returns the type of context node that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [MisspelledWord](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_MisspelledWord.htm) | Returns the text of the misspelled word, if any, that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [PageUrl](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_PageUrl.htm) | Returns the URL of the top level page that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [SelectionText](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_SelectionText.htm) | Returns the text of the selection, if any, that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [SourceUrl](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_SourceUrl.htm) | Returns the source URL, if any, for the element that the context menu was invoked on. Example of elements with source URLs are img, audio, and video. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [TypeFlags](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_TypeFlags.htm) | Returns flags representing the type of node that the context menu was invoked on. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [UnfilteredLinkUrl](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_UnfilteredLinkUrl.htm) | Returns the link URL, if any, to be used ONLY for "copy link address". We don't validate this field in the frontend process. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [XCoord](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_XCoord.htm) | Returns the X coordinate of the mouse where the context menu was invoked. Coords are relative to the associated RenderView's origin. |
| ![Public property](https://cefsharp.github.io/api/57.0.0/icons/pubproperty.gif "Public property") | [YCoord](https://cefsharp.github.io/api/57.0.0/html/P_CefSharp_IContextMenuParams_YCoord.htm) | Returns the Y coordinate of the mouse where the context menu was invoked. Coords are relative to the associated RenderView's origin. |

## Examples

```
void IContextMenuHandler.OnBeforeContextMenu(IWebBrowser browserControl, IBrowser browser, IFrame frame, IContextMenuParams parameters, IMenuModel model)
 {
     //Removing existing menu item
     model.Remove(CefMenuCommand.ViewSource); // Remove "View Source" option
     model.Remove(CefMenuCommand.Print);
     //Add new custom menu items
     model.AddItem((CefMenuCommand)ViewSource, "View Page Source");
     if (parameters.TypeFlags.HasFlag(ContextMenuType.Media) && parameters.HasImageContents)
     {
         if(OsirtHelper.HasJpegExtension(parameters.SourceUrl))
         {
             model.AddItem((CefMenuCommand)ViewImageExifData, "View image EXIF data");
         }
         model.AddItem((CefMenuCommand)MenuSaveImage, "Save image");
         model.AddItem((CefMenuCommand)CopyImgLocation, "Copy image location to clipboard");
         model.AddItem((CefMenuCommand)ReverseImageSearch, "Reverse image search using TinEye");
     }
     if(OsirtHelper.IsOnYouTube(browserControl.Address))
     {
         model.AddItem((CefMenuCommand)SaveYouTubeVideo, "Extract YouTube video");
     }
     if (OsirtHelper.IsOnFacebook(browserControl.Address))
     {
         model.AddItem((CefMenuCommand)ViewFacebookId, "Show Facebook profile ID");
     }
     if(!string.IsNullOrEmpty(parameters.UnfilteredLinkUrl))
     {
         model.AddItem((CefMenuCommand)26501, "Open link in new tab");
     }
 }
```

```
public bool OnBeforeContextMenu(IWebBrowser browser, IContextMenuParams parameters)
        {
            if (parameters.IsEditable)
                return true;

            return false;
        }
```

```
public bool OnBeforeContextMenu(IWebBrowser browser, IContextMenuParams parameters)
        {
            Console.WriteLine("Context menu opened");
            Console.WriteLine(parameters.MisspelledWord);

            return true;
        }
```

```
void IContextMenuHandler.OnBeforeContextMenu(IWebBrowser browserControl, IBrowser browser, IFrame frame, IContextMenuParams parameters, IMenuModel model)
        {
            Console.WriteLine("Context menu opened");
            Console.WriteLine(parameters.MisspelledWord);

            //To disable context mode then clear
            // model.Clear();
        }
```
