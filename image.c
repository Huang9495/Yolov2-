void draw_detections(image im, int num, float thresh, box *boxes, float **probs, float **masks, char **names, image **alphabet, int classes)
{
    int i;

    for (i = 0; i < num; ++i) {
        int class = max_index(probs[i], classes);
        float prob = probs[i][class];
        //*********** select-one ************//
        if (strcmp(names[class], "person") !=0 )
         {continue;}
        //*********** select-two ************//
         if(class != 2)
        {continue;}
        //************** end ***************//
        if (prob > thresh) {
            int width = im.h * .006;

            if (0) {
                width = pow(prob, 1. / 2.) * 10 + 1;
                alphabet = 0;
            }
            
            int offset = class * 123457 % classes;
            float red = get_color(2, offset, classes);
            float green = get_color(1, offset, classes);
            float blue = get_color(0, offset, classes);
            float rgb[3];

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = boxes[i];

            int left  = (b.x - b.w / 2.) * im.w;
            int right = (b.x + b.w / 2.) * im.w;
            int top   = (b.y - b.h / 2.) * im.h;
            int bot   = (b.y + b.h / 2.) * im.h;

            if (left < 0) left = 0;
            if (right > im.w - 1) right = im.w - 1;
            if (top < 0) top = 0;
            if (bot > im.h - 1) bot = im.h - 1;

            draw_box_width(im, left, top, right, bot, width, red, green, blue);

            if (alphabet) {
                image label = get_label(alphabet, names[class], (im.h * .03) / 10);
                draw_label(im, top + width, left, label, rgb);
                free_image(label);
            }
            if (masks) {
                image mask = float_to_image(14, 14, 1, masks[i]);
                image resized_mask = resize_image(mask, b.w * im.w, b.h * im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, left, top);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}
