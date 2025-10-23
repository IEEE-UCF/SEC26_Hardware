import cv2, numpy as np

low_yellow  = np.array([15, 125, 125])
high_yellow = np.array([35, 255, 255])

MIN_AREA = 1000
TARGET_HUE, H_CHANGE = 25, 20
KERNEL = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (15, 15))

def process_frame(frame):
    """Return metadata array of an image:
      Center position(x,y)
      SIZE(w,h)
      LABEL(confidence)
      debugging"""

    # convert to hue saturation value
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # create mask for yellow objects
    mask = cv2.inRange(hsv, low_yellow, high_yellow)

    # clean up specks & fill small holes
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE,  KERNEL)

    # see the mask in color
    result = cv2.bitwise_and(frame, frame, mask, mask=mask)

    # find the contours(object's boundaries) of the mask
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    H = hsv[:,:,0]
    S = hsv[:,:,1]
    V = hsv[:,:,2]
    debug = frame.copy()
    metadata  = []

    for contour in contours:
        area = cv2.contourArea(contour)
        if area < MIN_AREA:
            continue
        x, y, w, h = cv2.boundingRect(contour)

        # create a mask for the object itself
        roi_mask = mask[y:y+h, x:x+w] == 255
        roi_H    = H[y:y+h, x:x+w]
        roi_S    = S[y:y+h, x:x+w]
        roi_V    = V[y:y+h, x:x+w]

        # --- Fill Ratio ---
        # N_mask / N_bbox
        # N_mask = number of pixels with mask value 255 inside box
        # N_bbox = w * h
        n_mask = np.sum(roi_mask == 255)
        n_bbox = w * h
        fill_ratio = n_mask / n_bbox

        yy = roi_H[roi_mask>0]

        if yy.size > 0:
            # --- Hue Accuracy ---

            # compute circular hue distance to target hue H_t
            # d_i = min(|H_i - H_t|, 180 - |H_i - H_t|)
            d = abs(yy.astype(np.int16) - TARGET_HUE)
            d_i = np.minimum(d, 180 - d).astype(np.float32)
            # convert to [0-1] match score
            # hue_i = max(0, 1 - d_i / H_CHANGE)          H_CHANGE = maxium acceptable hue devation
            hue_i = np.maximum(0.0, 1.0 - (d_i / H_CHANGE)).mean()

            # Average over all mask pixels
            # hue_acc = 1 / (n_mask) * sum(hue_i)
            # hue_acc = (1 / n_mask) * hue_i

            # --- Saturation and brightness ---
            sat = roi_S[roi_mask>0].astype(np.float32) / 255
            bri = roi_V[roi_mask>0].astype(np.float32) / 255
            sv_boost = ((sat + bri)/2).mean()

            # --- Confidence ---

            confidence = ((0.0 * fill_ratio) + (0.9 * hue_i) + (0.1 * sv_boost)) * 100

            # Append the metadata
            metadata.append({
                                "Center Position": (float(x + w/2.0), float(y + h/2.0)),
                                "Size": (w, h),
                                "Label": float(confidence)
                            })

    return metadata, debug
