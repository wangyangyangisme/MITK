/*=========================================================================
 
 Program:   openCherry Platform
 Language:  C++
 Date:      $Date$
 Version:   $Revision$
 
 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.
 
 =========================================================================*/

#ifndef CHERRYPAGESELECTIONSERVICE_H_
#define CHERRYPAGESELECTIONSERVICE_H_

#include "../cherryISelectionService.h"
#include "../cherryIWorkbenchPage.h"

#include "cherryPagePartSelectionTracker.h"

#include <map>

namespace cherry
{

/**
 * \ingroup org_opencherry_ui_internal
 * 
 * The selection service for a page
 */
class PageSelectionService : public ISelectionService {

private:
  
    IWorkbenchPage::Pointer page;
  
    /** 
     * The list of selection listeners (not per-part).
     */
    //private ListenerList listeners = new ListenerList();

    /** 
     * The list of post selection listeners (not per-part).
     */
    //private ListenerList postListeners = new ListenerList();

    ISelectionService::SelectionEvents selectionEvents;
  
    /**
     * The currently active part.
     */
    IWorkbenchPart::Pointer activePart;

    /**
     * The active part's selection provider, remembered in case the part 
     * replaces its selection provider after we hooked a listener.
     */
    ISelectionProvider::Pointer activeProvider;

    /**
     * Map from part id (String) to per-part tracker (AbstractPartSelectionTracker).
     */
    std::map<std::string, PagePartSelectionTracker::Pointer> perPartTrackers;
    
    /**
     * Sets the page.
     */
    void SetPage(IWorkbenchPage::Pointer p);
    
    struct SelectionListener : public ISelectionChangedListener
    {
      SelectionListener(PageSelectionService* tracker);
      
      void SelectionChanged(SelectionChangedEvent::Pointer event);
      
      PageSelectionService* m_SelectionService;
    };
    
    friend class PostSelectionListener;
    
    
    struct PostSelectionListener : public ISelectionChangedListener
    {
      PostSelectionListener(PageSelectionService* tracker);
      
      void SelectionChanged(SelectionChangedEvent::Pointer event);
      
      PageSelectionService* m_SelectionService;
    };
    
    friend struct PostSelectionListener;
    
    /**
     * The JFace selection listener to hook on the active part's selection provider.
     */
    ISelectionChangedListener::Pointer selListener;

    /**
     * The JFace post selection listener to hook on the active part's selection provider.
     */
    ISelectionChangedListener::Pointer postSelListener;
    
public:
  
    /**
     * Creates a new SelectionService.
     */
    PageSelectionService(IWorkbenchPage::Pointer p);
    
    SelectionEvents& GetSelectionEvents(const std::string& partId = "");

    /* (non-Javadoc)
     * Method declared on ISelectionService.
     */
    void AddSelectionListener(ISelectionListener::Pointer l);

    /* (non-Javadoc)
     * Method declared on ISelectionService.
     */
    void AddSelectionListener(const std::string& partId, ISelectionListener::Pointer listener);

    /* (non-Javadoc)
     * Method declared on ISelectionService.
     */
    void AddPostSelectionListener(ISelectionListener::Pointer l);

    /* (non-Javadoc)
     * Method declared on ISelectionService.
     */
    void AddPostSelectionListener(const std::string& partId,
            ISelectionListener::Pointer listener);

    /* (non-Javadoc)
     * Method declared on ISelectionService.
     */
    void RemoveSelectionListener(ISelectionListener::Pointer l);

    /*
     * (non-Javadoc)
     * Method declared on ISelectionListener.
     */
    void RemovePostSelectionListener(const std::string& partId,
            ISelectionListener::Pointer listener);

    /* (non-Javadoc)
     * Method declared on ISelectionService.
     */
    void RemovePostSelectionListener(ISelectionListener::Pointer l);

    /*
     * (non-Javadoc)
     * Method declared on ISelectionListener.
     */
    void RemoveSelectionListener(const std::string& partId,
            ISelectionListener::Pointer listener);

    
protected:
  
  /**
   * Returns the page.
   */
  IWorkbenchPage::Pointer GetPage();
  
    /**
     * Fires a selection event to the given listeners.
     * 
     * @param part the part or <code>null</code> if no active part
     * @param sel the selection or <code>null</code> if no active selection
     */
    void FireSelection(IWorkbenchPart::Pointer part, ISelection::Pointer sel);

    /**
     * Fires a selection event to the given listeners.
     * 
     * @param part the part or <code>null</code> if no active part
     * @param sel the selection or <code>null</code> if no active selection
     */
    void FirePostSelection(IWorkbenchPart::Pointer part,
            ISelection::Pointer sel);

    /**
     * Returns the per-part selection tracker for the given part id.
     * 
     * @param partId part identifier
     * @return per-part selection tracker
     */
    PagePartSelectionTracker::Pointer GetPerPartTracker(const std::string& partId);

    /**
     * Creates a new per-part selection tracker for the given part id.
     * 
     * @param partId part identifier
     * @return per-part selection tracker
     */
    PagePartSelectionTracker::Pointer CreatePartTracker(
            const std::string& partId);

    
public:
  
    /**
     * Returns the selection.
     */
    ISelection::Pointer GetSelection();

    /*
     * @see ISelectionService#getSelection(String)
     */
    ISelection::Pointer GetSelection(const std::string& partId);

    /**
     * Sets the current-active part (or null if none)
     * 
     * @since 3.1 
     *
     * @param newPart the new active part (or null if none)
     */
    void SetActivePart(IWorkbenchPart::Pointer newPart);
    
//    /**
//     * Notifies the listener that a part has been activated.
//     */
//    public void partActivated(IWorkbenchPart newPart) {
//        // Optimize.
//        if (newPart == activePart)
//            return;
//
//        // Unhook selection from the old part.
//        reset();
//
//        // Update active part.
//        activePart = newPart;
//
//        // Hook selection on the new part.
//        if (activePart != null) {
//            activeProvider = activePart.getSite().getSelectionProvider();
//            if (activeProvider != null) {
//                // Fire an event if there's an active provider
//                activeProvider.addSelectionChangedListener(selListener);
//                ISelection sel = activeProvider.getSelection();
//                fireSelection(newPart, sel);
//                if (activeProvider instanceof IPostSelectionProvider)
//                    ((IPostSelectionProvider) activeProvider)
//                            .addPostSelectionChangedListener(postSelListener);
//                else
//                    activeProvider.addSelectionChangedListener(postSelListener);
//                firePostSelection(newPart, sel);
//            } else {
//                //Reset active part. activeProvider may not be null next time this method is called.
//                activePart = null;
//            }
//        }
//        // No need to fire an event if no active provider, since this was done in reset()
//    }
//
//    /**
//     * Notifies the listener that a part has been brought to the front.
//     */
//    public void partBroughtToTop(IWorkbenchPart newPart) {
//        // do nothing, the active part has not changed,
//        // so the selection is unaffected
//    }
//
//    /**
//     * Notifies the listener that a part has been closed
//     */
//    public void partClosed(IWorkbenchPart part) {
//        // Unhook selection from the part.
//        if (part == activePart) {
//            reset();
//        }
//    }
//
//    /**
//     * Notifies the listener that a part has been deactivated.
//     */
//    public void partDeactivated(IWorkbenchPart part) {
//        // Unhook selection from the part.
//        if (part == activePart) {
//            reset();
//        }
//    }
//
//    /**
//     * Notifies the listener that a part has been opened.
//     */
//    public void partOpened(IWorkbenchPart part) {
//        // Wait for activation.
//    }
//
//    /**
//     * Notifies the listener that a part has been opened.
//     */
//    public void partInputChanged(IWorkbenchPart part) {
//        // 36501 - only process if part is active
//        if (activePart == part) {
//            reset();
//            partActivated(part);
//        }
//    }
//
//    /**
//     * Resets the service.  The active part and selection provider are
//     * dereferenced.
//     */
//    public void reset() {
//        if (activePart != null) {
//            fireSelection(null, null);
//            firePostSelection(null, null);
//            if (activeProvider != null) {
//                activeProvider.removeSelectionChangedListener(selListener);
//                if (activeProvider instanceof IPostSelectionProvider)
//                    ((IPostSelectionProvider) activeProvider)
//                            .removePostSelectionChangedListener(postSelListener);
//                else
//                    activeProvider
//                            .removeSelectionChangedListener(postSelListener);
//                activeProvider = null;
//            }
//            activePart = null;
//        }
//    }

};

}

#endif /*CHERRYPAGESELECTIONSERVICE_H_*/
