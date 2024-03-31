using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DirectionColliders : MonoBehaviour
{
    public bool Player = true;
    private bool initialized = false;
    private void OnTriggerEnter(Collider other) {
        if (GameManager.Instance.start) {
            if (Player) {
                if (!other.CompareTag("Player") && !other.CompareTag("Untagged") && !other.CompareTag("North") && !other.CompareTag("East") && !other.CompareTag("South") && !other.CompareTag("West")) {
                    //Debug.Log("Player Has Object Entering From " + gameObject.tag + ": " + other.tag);
                    gameObject.transform.parent.GetComponent<PlayerBehavior>().UpdatePositionalArray(gameObject.tag, other);
                }
            } else {
                if (!other.CompareTag("Walkable") && !other.CompareTag("Untagged") && !other.CompareTag("North") && !other.CompareTag("East") && !other.CompareTag("South") && !other.CompareTag("West")) {
                    //Debug.Log("Crate Has Object Entering From " + gameObject.tag + ": " + other.tag);
                    gameObject.transform.parent.GetComponent<CrateBehavior>().UpdatePositionalArray(gameObject.tag, other);
                }
            }
        }
    }

    private void OnTriggerExit(Collider other) {
        if (GameManager.Instance.start) {
            if (Player) {
                List<Collider> list = gameObject.transform.parent.GetComponent<PlayerBehavior>().GetPositionalArray(gameObject.tag);
                for (int i = 0; i < list.Count; i++) {
                    if (list[i] != null && other.CompareTag(list[i].tag)) {
                        //Debug.Log("Player Has Object Leaving From " + gameObject.tag + ": " + other.tag);
                        gameObject.transform.parent.GetComponent<PlayerBehavior>().RemoveFromPositionalArray(gameObject.tag, list[i]);
                    }
                }
            } else {
                List<Collider> list = gameObject.transform.parent.GetComponent<CrateBehavior>().GetPositionalArray(gameObject.tag);
                for (int i = 0; i < list.Count; i++) {
                    if (list[i] != null && other.CompareTag(list[i].tag)) {
                        //Debug.Log("Crate Has Object Leaving From " + gameObject.tag + ": " + other.tag);
                        gameObject.transform.parent.GetComponent<CrateBehavior>().RemoveFromPositionalArray(gameObject.tag, list[i]);
                    }
                }
            }
        }
    }

    private void OnTriggerStay(Collider other) {
        if (!initialized && GameManager.Instance.start) {
            initialized = true;
            if (Player) {
                if (!other.CompareTag("Player") && !other.CompareTag("Untagged") && !other.CompareTag("North") && !other.CompareTag("East") && !other.CompareTag("South") && !other.CompareTag("West")) {
                    gameObject.transform.parent.GetComponent<PlayerBehavior>().UpdatePositionalArray(gameObject.tag, other);
                }
            } else {
                if (!other.CompareTag("Walkable") && !other.CompareTag("Untagged") && !other.CompareTag("North") && !other.CompareTag("East") && !other.CompareTag("South") && !other.CompareTag("West")) {
                    gameObject.transform.parent.GetComponent<CrateBehavior>().UpdatePositionalArray(gameObject.tag, other);
                }
            }
        }
    }
}
